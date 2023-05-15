#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "mem.h"
#include "strlib.h"
#include "mathlib.h"
#include "parser.h"
#include "url.h"

void
url_clear(url_t *url)
{
    if (url->username != NULL) {
	mem_free(url->username);
	url->username = NULL;
    }
    if (url->password != NULL) {
	mem_free(url->password);
	url->password = NULL;
    }
    if (url->protocol != NULL) {
	mem_free(url->protocol);
	url->protocol = NULL;
    }
    if (url->hostname != NULL) {
	mem_free(url->hostname);
	url->hostname = NULL;
    }
    if (url->service != NULL) {
	mem_free(url->service);
	url->service = NULL;
    }
    if (url->filename != NULL) {
	mem_free(url->filename);
	url->filename = NULL;
    }
}

void
url_free(url_t *url)
{
    url_clear(url);
    mem_free(url);
}

void
url_print(url_t *url)
{
    printf("[URL]\n");
    if (url->protocol != NULL)
	printf("protocol: %s\n", url->protocol);
    if (url->hostname != NULL)
	printf("hostname: %s\n", url->hostname);
    if (url->username != NULL)
	printf("username: %s\n", url->username);
    if (url->password != NULL)
	printf("password: %s\n", url->password);
    if (url->service != NULL)
	printf("service: %s\n", url->service);
    if (url->filename != NULL)
	printf("filename: %s\n", url->filename);
}

#define MODE_PROTO 1
#define MODE_USER  2
#define MODE_PASS  3
#define MODE_HOST  4
#define MODE_PORT  5
#define MODE_FILE  6
void
url_set(url_t *url, char *txt)
{
    parser_t *psr;
    int mode = MODE_PROTO;
    int slash_count = 0;
    string_t *file;

    url_clear(url);

    if (url->url != NULL)
	mem_free(url->url);

    url->url = dup(txt);

    psr = parser_new();
    parser_set_delimiters(psr, ":/@");
    parser_append(psr, txt);
    while (! psr->eof) {
	char *str;
	char c;

	if (mode == MODE_FILE) {
	    // we can skip some iterations by filling in the file contents here
	    str = parser_remainder(psr);
	    if (str == NULL) {
		url->filename = dup("/");
	    } else {
		file = str_new(str, len(str));
		free(str);

		// prepend / character if nessecary
		if (file->txt[0] != '/')
		    str_insert_char_at(file, '/', 0);

		url->filename = dup(file->txt);
		str_free(file);
	    }
	    break;
	}

	// search for the next token
	parser_next(psr);

	// save a few checks in the switch statement below
	if (psr->res->len == 0 && mode != MODE_USER)
	    continue;

	str = psr->res->txt;
	c   = psr->c;
	switch (mode) {
	    case MODE_PROTO:
		if (c == ':') {
		    // http: or file:
		    url->protocol = dup(str);
		    mode++;
		} else if (psr->eof) {
		    // foo.com
		    url->hostname = dup(str);
		    mode = MODE_FILE;
		}
		break;
	    case MODE_USER:
		if (c == ':') {
		    // either username or port determine which
		    parser_t *peek;
		    parser_peek(psr, &peek);
		    if (peek == NULL)
			goto end;

		    if (peek->c == '@') {
			// http://user:pass@host
			url->username = dup(str);
			mode++;
		    } else {
			// http://host:port
			url->hostname = dup(str);
			mode = MODE_PORT;
		    }

		    parser_destroy(peek);
		} else if (c == '/') {
		    if (psr->res->len == 0) {
			// http:// or file:///
			slash_count++;
		    } else {
			// http://foo/
			url->hostname = dup(str);
			mode = MODE_FILE;
		    }
		} else if (c == '@') {
		    // username with no password
		    url->username = dup(str);
		    mode = MODE_HOST;
		}
		break;
	    case MODE_PASS:
		if (c != '@')
		    goto end;
		// http://user:password@
		url->password = dup(str);
		mode++;
		break;
	    case MODE_HOST:
		url->hostname = dup(str);
		mode++;
		break;
	    case MODE_PORT:
		// http://hostname:port/
		if (! (psr->eof || c == '/'))
		    goto end;
		url->service = dup(str);
		mode++;
		break;
/*
	    case MODE_FILE:
		printf("file part: [%c] [%s]\n", c, str);
		break;
*/
	    default:
		assert(0);
		break;
	}
    }

    // fill in any missing data with defaults
    if (url->protocol == NULL)
	url->protocol = dup("http");
    if (url->service == NULL)
	url->service = dup("www");
    if (url->filename == NULL)
	url->filename = dup("/");
end:
    parser_destroy(psr);

    return;
}

url_t *
url_new(char *txt)
{
    url_t *url;

    url = mem_new0(url_t);
    url_set(url, txt);

    return url; 
}

#ifdef URL_MAIN
int
main(int argc, char **argv)
{
    char *url1 = "http://nexus6/";
    char *url2 = "http://google.com/?q=foo&p=2";
    char *url3 = "foo.com";
    char *url4 = "http://username:password@foo.com";
    char *url5 = "http://foo.com:80";
    char *url6 = "http://username:password@foo.com/asd/asd/asd.html?foo=bar=baz:/";
    char *url7 = "https://username:password@foo.com:443/asd/asd/asd.html?foo=bar=baz:/";
    url_t *url;

#if 1
    url = url_new(url1);
    url_print(url);
#endif

#if 1
    url_set(url, url2);
    url_print(url);
#endif

#if 1
    url_set(url, url3);
    url_print(url);
#endif

#if 1
    url_set(url, url4);
    url_print(url);
#endif

#if 1
    url_set(url, url5);
    url_print(url);
#endif

#if 1
    url_set(url, url6);
    url_print(url);
#endif

#if 1
    url_set(url, url7);
    url_print(url);
#endif

    return 0;
}
#endif
