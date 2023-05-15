#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "mem.h"
#include "strlib.h"
#include "mathlib.h"
#include "hash.h"
#include "tcp_unix.h"
#include "url.h"
#include "http.h"

http_req_t *
http_req_new(char *url)
{
    http_req_t *req;

    req = mem_malloc0(sizeof(http_req_t));
    req->url = url_new(url);
    req->vars = hash_new(&mem_free,&mem_free);
    req->cookies = hash_new(&mem_free,&mem_free);
    req->cl_headers = hash_new(&mem_free,&mem_free);
    req->sv_headers = hash_new(&mem_free,&mem_free);
    req->data = str_new(NULL, 0);
    req->buf = str_new(NULL, 0);

    char *key = "Accept";
    char *val = "*/*";
    hash_set(req->cl_headers, key, len(key), val, len(val));

    return req;
}

void
http_req_set_url(http_req_t *req, char *url)
{
    assert(req != NULL);

    if (req->url != NULL)
	url_free(req->url);

    req->url = url_new(url);
}

void
http_req_set_user_agent(http_req_t *req, char *user_agent)
{
    assert(req != NULL);
    assert(req->cl_headers != NULL);

    char *key = "User-Agent";
    hash_set(req->cl_headers, key, len(key), user_agent, len(user_agent));
}

void
http_req_destroy(http_req_t *req)
{
    hash_destroy(req->vars);
    hash_destroy(req->cookies);
    hash_destroy(req->cl_headers);
    hash_destroy(req->sv_headers);
    str_free(req->data);
    mem_free(req->url);
    mem_free(req);
}

/* perform get request & parse headers */
boolean
http_req_get_start(http_req_t *req)
{
    char *get_fmt = 
	"GET %s HTTP/1.0\r\n"
	"Host: %s\r\n";
//	"User-Agent: %s\r\n"
//	"Accept: */*\r\n\r\n";

    char buf[BUFSIZ];
    int i, n;
    char **keys;
    string_t *get;

    // check inputs
    if (req->url->hostname == NULL || req->url->service == NULL || req->url->filename == NULL)
	return false;

    // connect to web server
    req->fd = tcp_connect(req->url->hostname, req->url->service);
    if (req->fd <= 0)
	return false;

    printf("fd: %d\n", req->fd);

    // create GET request
    snprintf(buf, BUFSIZ, get_fmt, req->url->filename, req->url->hostname);
    get = str_new(buf, len(buf));

    // client headers
    keys = (char **)hash_get_keys_array(req->cl_headers, &n);
    for (i = 0; i < n; i++) {
	char *val;
       	hash_get(req->cl_headers, keys[i], len(keys[i]), &val, NULL);
	snprintf(buf, BUFSIZ, "%s: %s\r\n", keys[i], val);
	str_append_str(get, buf, len(buf));
    }

    str_append_str(get, "\r\n", len("\r\n"));

    // send GET request
    if ((n = write(req->fd, get->txt, get->len)) <= 0) {
	fprintf(stderr, "write failed: %s\n", strerror(errno));
	return false;
    }

    req->parsing_headers = true;
    req->line_num = 0;

    return true;
}

void
http_req_parse_data(http_req_t *req, char *buf, int len)
{
    if (req->save_to_mem == true)
	str_append_str(req->data, buf, len);

    if (req->data_cb)
	req->data_cb(req, buf, len);
}

void
http_req_parse(http_req_t *req)
{
    char **results;
    string_t *res;
    int complete;
    int n;

    // if we are already done parsing headers, process data portion
    if (req->parsing_headers == false) {
	http_req_parse_data(req, req->buf->txt, req->buf->len);
	str_clear(req->buf);
	return;
    }

    while ((res = str_read_line(req->buf, &complete)) != NULL) {
	req->line_num++;

	//	    printf("line[%d]: %s\n", complete, res->txt);

	// Server's HTTP response code
	if (req->line_num == 1) {
	    if (str_len(res) >= 4 && eqn(res->txt, "HTTP", 4)) {
		printf("RESPONSE: %s\n", res->txt);
		goto next;
	    } else {
		fprintf(stderr, "ERROR: Standard HTTP response not received\n");
		exit(1);
	    }
	}

	// Blank line signals end of headers
	if (res->len == 0) {
	    //		printf("done parsing headers\n");
	    string_t *remainder;

	    req->parsing_headers = false;

	    // call headers finished callback
	    if (req->headers_cb != NULL) {
		req->headers_cb(req);
	    }

	    // if there is data left in the buffer process it
	    if ((remainder = str_remainder(req->buf)) != NULL) {
		http_req_parse_data(req, remainder->txt, remainder->len);
		str_free(remainder);
	    }

	    // make sure buffer is empty
	    str_clear(req->buf);

	    str_free(res);

	    break;
	}

	// parse header & add it to server header hash
	results = split(res->data, ":", 2, &n);
	if (n != 2) {
	    fprintf(stderr, "error parsing header: %s\n", res->data);
	    exit(1);
	}

	// strip leading & trailing whitespace & from value
	strip(results[1], STRIP_START, STRIP_WHITESPACE);
	strip(results[1], STRIP_END, STRIP_WHITESPACE|STRIP_NEWLINE);

	// create header entry
	hash_set(req->sv_headers, results[0], len(results[0]), results[1], len(results[1]));

next:
	req->line_num++;
	str_free(res);

	if (complete == 0)
	    break;
    }
}

/* read & process input from network, returns return value from read */
int
http_req_read(http_req_t *req)
{
    return str_append_from_fd(req->buf, req->fd);
}

/* blocking get, returns NULL on error */
string_t *
http_req_get_full(http_req_t *req)
{
    int ret;

    req->save_to_mem = true;

    if (http_req_get_start(req) != true) {
	return NULL;
    }

    while ((ret = http_req_read(req)) > 0)
	http_req_parse(req);

    return req->data;
}

#ifdef HTTP_MAIN
int
main(int argc, char **argv)
{
    http_req_t *req;
    char *url = "http://nexus6/";
    char *user_agent = "Space Combat Simulator";
    string_t *page;

    mem_init();

    // grab web page
    req = http_req_new(url);
    http_req_set_user_agent(req, user_agent);
    page = http_req_get_full(req);

    if (page != NULL) {
	printf("[HEADERS]\n");
	hash_print(req->sv_headers);

	printf("[DATA]\n");
	printf("%s\n", page->txt);
    } else {
	fprintf(stderr, "ERROR could not fetch web page\n");
	exit(1);
    }

    return 0;

}
#endif
