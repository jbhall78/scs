#ifndef ERROR_H
#define ERROR_H

typedef struct {
    char *msg;
} err_t;

void error_set(err_t **dst, char *fmt, ...);
void error_free(err_t *err);
void error_propagate(err_t **dst, err_t *err);

#endif
