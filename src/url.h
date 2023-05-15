#ifndef URL_H
#define URL_H

typedef struct {
    char *url;
    char *username;
    char *password;
    char *protocol;
    char *hostname;
    char *service;
    char *filename;
} url_t;

void url_free(url_t *url);

void url_print(url_t *url);

url_t *url_new(char *url);

#endif
