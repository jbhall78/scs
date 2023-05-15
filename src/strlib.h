#ifndef STRLIB_H
#define STRLIB_H

#include <string.h>

/* standard (char *) string functions */
#define eq(s1,s2) (strcmp(s1,s2) == 0)		// a little saner than strcmp
#define eqn(s1,s2,n) (strncmp(s1,s2,n) == 0)	// ...
#define len(s) (s == NULL) ? 0 : strlen(s)	// a little safer that strlen
#define dup(s) strdup(s)

#define STRIP_START      0
#define STRIP_END        1
#define STRIP_NEWLINE	 (1 << 0)
#define STRIP_WHITESPACE (1 << 1)

void strip(char *str, int position, int flags);
char **split(char *str, char *delim, int max, int *items);

/* enhanced (string_t *) functions */
typedef struct {
    union {		// aliases for actual string data
    	char *data;
	char *str;
	char *s;
	char *txt;
    };
    // enum { ASCII, UNICODE, UTF8 } internal_format;
    char *iter;		// saved position during parsing functions
    size_t len;	// length of string
    size_t alloc;	// allocated memory
} string_t;

#define str_eq(s1,s2) (s1->len == s2->len) ? (strcmp(s1->data, s2->data) == 0) : 0
#define str_eqn(s1,s2,n) (s1->len >= n && s2->len >= n) ? (strncmp(s1->data, s2->data, n) == 0) : 0
#define str_len(s) s->len
#define str_dup(a) str_new(a->data, a->len)

string_t *str_new(char *buf, size_t len);
void str_free(string_t *str);
void str_clear(string_t *str);
void str_alloc(string_t *str, int alloc);
void str_append_char(string_t *str, char c);
// void str_append_unicode(string_t *str, int);
// the following should work fine with unicode
void str_append_str(string_t *str, char *buf, int len);

int str_append_from_fd(string_t *str, int fd);
int str_append_from_fread(string_t *str, FILE *file);
char *str_append_from_fgets(string_t *str, FILE *file);
// the following need to be updated for unicode/utf8
void str_insert_char_at(string_t *str, char c, int pos);
void str_insert_str_at(string_t *str, char *buf, int len, int pos);
void str_delete_at(string_t *str, int len, int pos);
void str_truncate_from(string_t *str, int pos);

void str_reset_iter(string_t *str);
string_t *str_read(string_t *str, int n);

// the following needs to be updated for unicode internal format (not utf-8)
string_t *str_read_line(string_t *str, int *complete);

string_t *str_remainder(string_t *str);


#endif
