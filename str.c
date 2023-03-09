#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <unistd.h>

#include "mem.h"
#include "strlib.h"

/*
 * Pascalish String Functions
 *
 * Can hold data which contains NULL characters
 */
#ifndef MIN
#define MIN(a,b) (a < b) ? a : b
#endif

#ifndef MAX
#define MAX(a,b) (a > b) ? a : b
#endif

string_t *
str_new(char *buf, size_t len)
{
    string_t *str;

    str = mem_malloc(sizeof(string_t));
    memset(str, 0, sizeof(string_t));

    if (buf != NULL && len > 0) {
#if 0
	size_t min;
	size_t n = strlen(buf);

	min = MIN(len, n);

#else
	size_t min = len;
#endif
	str->data = mem_malloc(min+1);
	str->alloc = min+1;

	str->data[min] = 0x0;

	memcpy(str->data, buf, min); 
	str->len = min;
    } else if (buf == NULL && len == 0) {
	str->data = mem_malloc(1);	// for append operations
	str->alloc = 1;
	str->data[0] = 0x0;
    }

    str->iter = str->data;

    return str;
}

void
str_free(string_t *str)
{
    mem_free(str->data);
    mem_free(str);
}

void
str_clear(string_t *str)
{
    str->len = 0;
    str->iter = str->data;
//    memset(str->data, 0, str->alloc);
    str->data[0] = 0x0; // faster
}

// ensure string has alloc'd at least len size bytes
void
str_alloc(string_t *str, int alloc)
{
    char *save;
    int pos;

    if (alloc >= str->alloc) {
	pos = str->iter - str->data;
#ifdef DEBUG
	str->alloc += MAX(1,alloc-str->alloc);	// allocate at least 100 bytes // 1= force to use logic
#else
	str->alloc += MAX(100,alloc-str->alloc);	// allocate at least 100 bytes
#endif
	save = str->data;
	str->data = mem_malloc(str->alloc);
	assert(str->data != NULL);
	memcpy(str->data, save, str->len);
	str->data[str->len] = 0x0;
	mem_free(save);
	str->iter = str->data + pos;
    }
}

/*
 * Append Functions
 */
void
str_append_char(string_t *str, char c)
{
    str_alloc(str, str->len + 1);

    str->data[str->len] = c;
    str->len++;
    str->data[str->len] = 0x0;
}

void
str_append_str(string_t *str, char *buf, int len)
{
    char *p;

    str_alloc(str, str->len + len + 1);

    p = &str->data[str->len];
    memcpy(p, buf, len);
    str->len += len;
    str->data[str->len] = 0x0;
}

/*
 * File related append functions
 */
int
str_append_from_fd(string_t *str, int fd)
{
    char buf[BUFSIZ];
    int n;

    n = read(fd, buf, BUFSIZ);
    if (n > 0)
	str_append_str(str, buf, n);

    return n;
}

int
str_append_from_fread(string_t *str, FILE *file)
{
    char buf[BUFSIZ];
    int n;

    n = fread(buf, 1, BUFSIZ, file);
    if (n > 0)
	str_append_str(str, buf, n);

    return n;
}

char *
str_append_from_fgets(string_t *str, FILE *file)
{
    char buf[BUFSIZ];
    char *ret;

    ret = fgets(buf, BUFSIZ, file);
    if (ret != NULL)
	str_append_str(str, buf, strlen(buf));

    return ret;
}

/*
 * Insert Functions
 */
void
str_insert_char_at(string_t *str, char c, int pos)
{
    int i;

    if (pos < 0 || pos > str->len)
	return;

    str_alloc(str, str->len + 1);

    // shift end of string by one char
    for (i = str->len+1; i > pos; i--)
	str->data[i] = str->data[i-1];

    // set character
    str->data[pos] = c;

    // increase length
    str->len++;
}

void
str_insert_str_at(string_t *str, char *buf, int len, int pos)
{
    char *p1, *p2;
    int end_size;
    char *save;

    if (pos < 0 || pos > str->len)
	return;

    str_alloc(str, str->len + len + 1);

    end_size = str->len - pos;
    save = mem_malloc(end_size);
    p1 = &str->data[pos];
    memcpy(save, p1, end_size);
    p2 = &str->data[pos+len];
    memcpy(p2, save, end_size);
    mem_free(save);

    // insert string
    memcpy(p1, buf, len);

    // increase length
    str->len += len;
}

/*
 * Delete Functions
 */
void
str_delete_at(string_t *str, int len, int pos)
{
    char *p1, *p2;
    char *save;
    int end_size;

#if 1
    assert(pos >= 0);
    assert(pos < str->len);
    assert(len <= str->len);
#else
    if (pos < 0 || pos >= str->len || len > str->len)
        return;
#endif

    end_size = str->len - (pos+len);
    save = mem_malloc(end_size);

    p1 = &str->data[pos+len];
    memcpy(save, p1, end_size);

    p2 = &str->data[pos];
    memcpy(p2, save, end_size);
    free(save);

    str->len -= len;
    str->data[str->len] = 0x0;
}

void
str_truncate_from(string_t *str, int pos)
{
    if (pos < 0 || pos > str->len)
	return;

    str->len = pos;
    str->data[str->len] = 0x0;
}

/*
 * Iterating Functions
 */
void
str_reset_iter(string_t *str)
{
    str->iter = str->data;
}

string_t *
str_remainder(string_t *str)
{
    int len = str->len - (str->iter - str->data);
    if (len <= 0) 
	return NULL;
    string_t *s = str_new(str->iter, len);
    return s;
}

// attempt to read n bytes from str buffer
// useful for fixed packet sizes
// puts results into buf
// returns NULL if there arent enough bytes in buffer or a new string containing the bytes 
string_t *
str_read(string_t *str, int n)
{
    char *p1;
    int pos;
    string_t *s;

    p1 = str->iter; 
    pos = str->iter - str->data;

    if (pos + n > str->len)
	return NULL;

    s = str_new(NULL, 0);

    str_alloc(s, n+1);
    memcpy(s->data, p1, n);
    s->len = n;
    s->data[s->len] = 0x0;

    str->iter += n;

    return s;
}

// read a line from string buffer
// complete = if string was a complete line or the remainder
string_t *
str_read_line(string_t *str, int *complete)
{
    char *p1;
    int pos;
    string_t *s;

    s = str_new(NULL, 0);

    p1 = str->iter; 
    pos = str->iter - str->data;

    // NULL is treated like a newline character
    // probably shouldn't be, if this causes problems, then just remove the '\0' tests it should work fine
    while (pos < str->len && *p1 != '\0' && *p1 != '\n' && !(pos + 1 < str->len && *p1 == '\r' && *(p1 + 1) == '\n')) {
	str_append_char(s, str->data[pos]);
	p1++;
	pos++;
    }

    if (pos < str->len) {
	if (complete != NULL)
	    *complete = 1;

	// set iterator to beginning of next line
	if (*p1 == '\0' || *p1 == '\n')
	    str->iter = p1 + 1;
	else if (*p1 == '\r' && *(p1 + 1) == '\n')
	    str->iter = p1 + 2;

	return s;
    } else {
	if (complete != NULL)
	    *complete = 0;

	if (s->len > 0)
	    return s;
	else  {
	    str_free(s);
	    return NULL;
	}
    }
}

#ifdef STR_MAIN
int
main(int argc, char **argv)
{
    string_t *s, *res;
    char *test = "test ";
    int complete;

    mem_init();

    s = str_new(NULL, 0);

    // append str
    str_append_str(s, test, len(test));
    printf(">>> %s\n", s->txt);

    // append char
    str_append_char(s, '2');
    printf(">>> %s\n", s->txt);
    str_append_char(s, '3');
    printf(">>> %s\n", s->txt);

    // insert char
    str_insert_char_at(s, '.', 0);
    printf(">>> %s\n", s->txt);
    str_insert_char_at(s, '1', 6);
    printf(">>> %s\n", s->txt);
    str_insert_char_at(s, '.', s->len);
    printf(">>> %s\n", s->txt);

    // insert string
    str_insert_str_at(s, "ing", len("ing"), 5);
    printf(">>> %s\n", s->txt);
    str_insert_str_at(s, "###", len("###"), 0);
    printf(">>> %s\n", s->txt);

    // another append string
    test = ".. worked!";
    str_append_str(s, test, len(test));
    printf(">>> %s\n", s->txt);
    str_insert_str_at(s, "@$%", len("###"), s->len);
    printf(">>> %s\n", s->txt);

    // delete stuff
    str_delete_at(s, 4, 0);
    printf(">>> %s\n", s->txt);
    str_truncate_from(s, 11);
    printf(">>> %s\n", s->txt);

    // read line
    str_free(s);
    test = 
	"test 123 abc\n"
	"foo bar baz\r\n"
	"null\0" "1234";
    s = str_new(test, strlen(test) + 5);
    while ((res = str_read_line(s, &complete)) != NULL) {
	printf("line[%d]: %s\n", complete, res->txt);

	str_free(res);

	if (complete == 0)
	    break;
    }
    str_free(s);

    // read
    test = 
	"1234"
	"5678"
	"90";
    s = str_new(test, strlen(test));
    while ((res = str_read(s, 4)) != NULL) {
	printf("data: %s\n", res->txt);
	str_free(res);
    }
    str_free(s);

    mem_print();

    return 0;
}
#endif
