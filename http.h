#ifndef HTTP_H
#define HTTP_H

#include "strlib.h"
#include "mathlib.h"
#include "hash.h"
#include "url.h"

typedef struct http_req_s http_req_t;

struct http_req_s {
    int fd;
    int sv_response;
    url_t *url;
    hash_t *vars;	// form variables
    hash_t *cookies;	// cookies
    hash_t *cl_headers; // headers client sends
    hash_t *sv_headers;	// headers from server 
    string_t *data;

    // parsing state information
    string_t *buf;
    boolean parsing_headers;
    boolean save_to_mem;
    int line_num;

    void (*data_finished_cb)(http_req_t *);	// called after full page has been retrieved
    void (*data_cb)(http_req_t *, char *buf, int len);		// called as data is obtained
    void (*headers_cb)(http_req_t *);		// called after headers have been retrieved
};

http_req_t *http_req_new(char *url);
void http_req_set_url(http_req_t *req, char *url);
void http_req_set_user_agent(http_req_t *req, char *user_agent);
void http_req_destroy(http_req_t *req);
boolean http_req_get_start(http_req_t *req);
void http_req_parse_data(http_req_t *req, char *buf, int len);
void http_req_parse(http_req_t *req);
int http_req_read(http_req_t *req);
string_t *http_req_get_full(http_req_t *req);

#endif
