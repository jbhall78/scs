#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "strlib.h"
#include "mathlib.h"

// parser
typedef struct {
    string_t *buf;	// text buffer to parse
    string_t *res;	// result
    string_t *delimiters; // delimiter characters
    size_t idx;		// current position index in text buffer
    char c;		// current character
    boolean eof;	// set when we are at end of text buffer
    boolean empty;	// set if result is empty
} parser_t;

/*
 * Create a new parser instance
 */
parser_t *parser_new(void);

void parser_set_delimiters(parser_t *psr, char *str);


void parser_append(parser_t *psr, char *str);

/*
 * Free the parser and all associated data
 */
void parser_destroy(parser_t *psr);

/*
 * Reset index position in input buffer along with mem_freeing any result data
 */
void parser_reset(parser_t *psr);

/*
 * Fill result buffer until one of chars[] or end of input buffer is reached
 */
void parser_next(parser_t *psr);

/*
 * Fill result buffer until end of input buffer is reached
 */
void parser_end(parser_t *psr);

void parser_peek(parser_t *psr, parser_t **res);

void parser_print(parser_t *psr);

char *parser_remainder(parser_t *psr);

#endif
