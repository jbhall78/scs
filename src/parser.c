#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "mem.h"
#include "strlib.h"
#include "mathlib.h"
#include "parser.h"

// then remove data from the buffer after it has been parsed

/*
 * Create a new parser instance
 */
parser_t *
parser_new(void)
{
    parser_t *psr = mem_new0(parser_t);

    //psr->buf = str_new(str, len(str));
    psr->buf = str_new(NULL, 0);
    psr->res = str_new(NULL, 0);

    return psr;
}

void
parser_set_delimiters(parser_t *psr, char *str)
{
    if (psr->delimiters != NULL)
	str_free(psr->delimiters);

    psr->delimiters = str_new(str, len(str));
}

void
parser_append(parser_t *psr, char *str)
{
    str_append_str(psr->buf, str, len(str));
}

/*
 * Free the parser and all associated data
 */
void
parser_destroy(parser_t *psr)
{
    str_free(psr->buf);
    psr->buf = NULL;

    str_free(psr->res);
    psr->res = NULL;

    mem_free(psr);
}

/*
 * Reset index position in input buffer along with mem_freeing any result data
 */
void
parser_reset(parser_t *psr)
{
    psr->idx = psr->c = psr->empty = psr->eof = 0;
    if (psr->res != NULL)
	str_free(psr->res);
    psr->res = NULL;
}

/*
 * Fill result buffer until one of chars[] or end of input buffer is reached
 */
void
parser_next(parser_t *psr)
{
    int i, j;

    psr->c = psr->empty = psr->eof = 0;

    str_clear(psr->res);

    for (i = psr->idx; i < psr->buf->len; i++) {
	char c = psr->buf->data[i];

//	if (c == '\n')
//	    continue;

	psr->idx = i+1;

	for (j = 0; j < psr->delimiters->len; j++) {
	    if (c == psr->delimiters->txt[j]) {
		psr->c = c;
		goto end;
	    }
	}

	str_append_char(psr->res, c);
    }

end:
    if (i == psr->buf->len)
	psr->eof = 1;

    if (psr->res->len == 0)
	psr->empty = 1;

    int max = 8192;
    if (psr->idx > max) {
	psr->idx -= max;
	str_delete_at(psr->buf, max, 0);
    }
}

/*
 * Fill result buffer until end of input buffer is reached
 */
void
parser_end(parser_t *psr)
{
    int i;

    if (psr->res != NULL)
	str_free(psr->res);

    psr->c = psr->empty = psr->eof = 0;

    psr->res = str_new(NULL, 0);

    for (i = psr->idx; i < psr->buf->len; i++) {
	char c = psr->buf->data[i];

	if (c == '\n')
	    continue;

	psr->idx = i+1;
	str_append_char(psr->res, c);
    }

    if (i == psr->buf->len)
	psr->eof = 1;

    if (psr->res->len == 0)
	psr->empty = 1;
}

void
parser_peek(parser_t *psr, parser_t **res)
{
    parser_t *p;

    // BUG: needs bounds checking
    p = parser_new();
    parser_set_delimiters(p, psr->delimiters->txt);
    parser_append(psr, &psr->buf->data[psr->idx]);
    parser_next(p);

    *res = p;
}

char *
parser_remainder(parser_t *psr)
{
    // BUG: needs bounds checking
    char *p = &psr->buf->data[psr->idx];
    if (psr->idx == psr->buf->len)
	return NULL;
    else 
	return strdup(p);
}

void
parser_print(parser_t *psr)
{
    printf("got: idx:[%ld] c:'%c' res:\"%s\" eof:[%s]\n", psr->idx, psr->c,
	    (psr->empty) ? "NULL" : psr->res->data,
	    (psr->eof ? "yes" : "no")
	  );
}

#ifdef PARSER_MAIN
int
main(int argc, char **argv)
{
    int opt;
    char *filename = NULL;
    int verbose = 0;
    parser_t *psr;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <string>\n", argv[0]);
	exit(1);
    }

    psr = parser_new(argv[1]);
    while (! psr->eof) {
	parser_next(psr, " [](),=:+|;");
	parser_print(psr);
    } 
    parser_destroy(psr);

    return 0;
}
#endif
