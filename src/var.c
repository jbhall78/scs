/*
 * Name: var.c
 * Description: Loosely typed variable API
 * Author: Jason Hall <jbhall78@gmail.com>
 *
 * Copyright (c) 2012 Jason Hall <jbhall78@gmail.com>
 * All Rights Reserved
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dbuf.h"
#include "var.h"

#include "var_auto.c"

typedef void (*conv_func_to_str_t)(var_t *v, char *buf, int len);
typedef void (*conv_func_to_blob_t)(var_t *v, char *buf, int len, int *size);

#define VAR_TYPES_MAX 255

// maps to type enum
conv_func_to_str_t var_to_str_tbl[VAR_TYPES_MAX] = {
    NULL, // 0
    bool_to_str,	// VAR_BOOL
    char8_to_str,	// VAR_CHAR8
    uchar8_to_str,	// VAR_UCHAR8
    str_to_str,		// VAR_STRING8
    int8_to_str,	// VAR_INT8
    uint8_to_str,	// VAR_UINT8
    int16_to_str,	// VAR_INT16
    uint16_to_str,	// VAR_UINT16
    int32_to_str,	// VAR_INT32
    uint32_to_str,	// VAR_UINT32
    int64_to_str,	// VAR_INT64
    uint64_to_str,	// VAR_UINT64
    float_to_str,	// VAR_FLOAT
    double_to_str,	// VAR_DOUBLE
    NULL,		// VAR_PTR
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// reserved: primitives
    NULL,		// user defined
    NULL,		// ...
};

void
var_to_str(var_t *var, char *buf, int len)
{
    conv_func_to_str_t conv_func = var_to_str_tbl[var->type];

    if (conv_func != NULL)
	conv_func(var,buf,len);
}

#if 0
void
bool_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%s", (v->b == true) ? "true" : "false");
}

void
bool_to_blob(var_t *v, char *buf, int len, int *size)
{
    int8_t i;

    if (len < 1)
	return;

    if (v->b == true)
	i = 1;
    else
	i = 0;

    set8(buf, i); 

    if (size != NULL)
	*size = 1;
}

void
char8_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%c", v->c8);
}

void
char8_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 1)
	return;

    set8(buf, v->c8); 

    if (size != NULL)
	*size = 1;
}

void
uchar8_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%c", v->uc8);
}

void
uchar8_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 1)
	return;

    set8(buf, v->c8); 

    if (size != NULL)
	*size = 1;
}

void
str_to_str(var_t *v, char *buf, int len)
{
    strncpy(buf, v->ptr, len);
}

void
str_to_blob(var_t *v, char *buf, int len, int *size)
{
    int s;
    s = snprintf(buf, len, "%s", (char *)v->ptr);
    if (size != NULL)
	*size = s+1;
}

void
int8_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%d", v->i8);
}

void
int8_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 1)
	return;

    set8(buf, v->i8); 
    *size = 1;
}

void
int16_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%d", v->i16);
}

void
int16_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 2)
	return;

    set16(buf, v->i16);
    if (size != NULL)
	*size = 2;
}

void
int32_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%d", v->i32);
}

void
int32_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 4)
	return;

    set32(buf, v->i32);
    if (size != NULL)
	*size = 4;
}

void
int64_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%lld", v->i64);
}

void
int64_to_blob(var_t *v, char *buf, int len, int *size)
{
    if (len < 8)
	return;

    set64(buf, len);

    if (size != NULL)
	*size = 8;
}

void
uint8_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%u", v->ui8);
}

void
uint8_to_str(var_t *v, char *buf, int len, int *size)
{
    int s;
    s = snprintf(buf, len, "%u", v->ui8);
    if (size != NULL)
	*size = s;
}

void
uint16_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%u", v->ui16);
}

void
uint16_to_str(var_t *v, char *buf, int len, int *size)
{
    int s;
    s = snprintf(buf, len, "%u", v->ui16);
    if (size != NULL)
	*size = s;
}

void
uint32_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%u", v->ui32);
}

void
uint32_to_str(var_t *v, char *buf, int len, int *size)
{
    int s;
    s = snprintf(buf, len, "%u", v->ui32);
    if (size != NULL)
	*size = s;
}

void
uint64_to_str(var_t *v, char *buf, int len)
{
    snprintf(buf, len, "%llu", v->ui64);
}

void
uint64_to_blob(var_t *v, char *buf, int len, int *size)
{
    int s;
    s = snprintf(buf, len, "%llu", v->ui64);
    if (size != NULL)
	*size = s;
}

void
float_to_str(var_t *v, char *buf, int len)
{
    // use %g so whole numbers are displayed as a whole number
    snprintf(buf, len, "%g", v->flt);
    if (strchr(buf, 'e') != NULL) {
        // exponent notation detected, use alternate format.
        // exponents appear on the right side of the text
        // entry boxes and can be missed by users, making them
        // think the number is smaller or larger, so use the
        // %f number format instead for these numbers
        snprintf(buf, len, "%f", v->flt);
    }
}

void
double_to_str(var_t *v, char *buf, int len)
{
    // use %g so whole numbers are displayed as a whole number
    snprintf(buf, len, "%g", v->dbl);
    if (strchr(buf, 'e') != NULL) {
        // exponent notation detected, use alternate format.
        // exponents appear on the right side of the text
        // entry boxes and can be missed by users, making them
        // think the number is smaller or larger, so use the
        // %f number format instead for these numbers
        snprintf(buf, len, "%f", v->dbl);
    }
}

void
int32_from_str(var_t *v, char *buf)
{
    v->i32 = atoi(buf);
}

/*
 * Stack allocated for easy & temporary usage
 */
inline var_t *
bools(bool val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_BOOL;
    v->b = val;
    v->size = sizeof(bool);
    return v;
}

inline var_t *
char8s(int8_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_CHAR8;
    v->c8 = val;
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
string8s(char *val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_STRING8;
    // don't bother to copy the entire string here
    v->s8 = val;
    v->len = strlen(val);
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
uchar8s(uint8_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_UCHAR8;
    v->uc8 = val;
    v->size = sizeof(uint8_t);
    return v;
}

inline var_t *
int8s(int8_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_INT8;
    v->i8 = val;
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
uint8s(uint8_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_UINT8;
    v->ui8 = val;
    v->size = sizeof(uint8_t);
    return v;
}

inline var_t *
int16s(int16_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_INT16;
    v->i16 = val;
    v->size = sizeof(int16_t);
    return v;
}

inline var_t *
uint16s(uint16_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_UINT16;
    v->ui16 = val;
    v->size = sizeof(uint16_t);
    return v;
}

inline var_t *
int32s(int32_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_INT32;
    v->i32 = val;
    v->size = sizeof(int32_t);
    return v;
}

inline var_t *
uint32s(uint32_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_UINT32;
    v->ui32 = val;
    v->size = sizeof(uint32_t);
    return v;
}

inline var_t *
int64s(int64_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_INT64;
    v->i64 = val;
    v->size = sizeof(int64_t);
    return v;
}

inline var_t *
uint64s(uint64_t val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_UINT64;
    v->ui64 = val;
    v->size = sizeof(uint64_t);
    return v;
}

inline var_t *
floats(float val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_FLOAT;
    v->flt = val;
    v->size = sizeof(float);
    return v;
}

inline var_t *
doubles(double val)
{
    var_t *v = alloca(sizeof(var_t));
    v->type = VAR_DOUBLE;
    v->dbl = val;
    v->size = sizeof(double);
    return v;
}

/*
 * Heap allocated for longer term storage (post caller return)
 */
inline var_t *
boolh(bool val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_BOOL;
    v->b = val;
    v->size = sizeof(bool);
    return v;
}

inline var_t *
char8h(int8_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_CHAR8;
    v->c8 = val;
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
string8h(char *val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_STRING8;
    // don't bother to copy the entire string here
    v->s8 = val;
    v->len = strlen(val);
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
uchar8h(uint8_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_UCHAR8;
    v->uc8 = val;
    v->size = sizeof(uint8_t);
    return v;
}

inline var_t *
int8h(int8_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_INT8;
    v->i8 = val;
    v->size = sizeof(int8_t);
    return v;
}

inline var_t *
uint8h(uint8_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_UINT8;
    v->ui8 = val;
    v->size = sizeof(uint8_t);
    return v;
}

inline var_t *
int16h(int16_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_INT16;
    v->i16 = val;
    v->size = sizeof(int16_t);
    return v;
}

inline var_t *
uint16h(uint16_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_UINT16;
    v->ui16 = val;
    v->size = sizeof(uint16_t);
    return v;
}

inline var_t *
int32h(int32_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_INT32;
    v->i32 = val;
    v->size = sizeof(int32_t);
    return v;
}

inline var_t *
uint32h(uint32_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_UINT32;
    v->ui32 = val;
    v->size = sizeof(uint32_t);
    return v;
}

inline var_t *
int64h(int64_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_INT64;
    v->i64 = val;
    v->size = sizeof(int64_t);
    return v;
}

inline var_t *
uint64h(uint64_t val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_UINT64;
    v->ui64 = val;
    v->size = sizeof(uint64_t);
    return v;
}

inline var_t *
floath(float val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_FLOAT;
    v->flt = val;
    v->size = sizeof(float);
    return v;
}

inline var_t *
doubleh(double val)
{
    var_t *v = malloc(sizeof(var_t));
    v->type = VAR_DOUBLE;
    v->dbl = val;
    v->size = sizeof(double);
    return v;
}
#endif

void
test_inline(var_t *v1, var_t *v2, var_t *v3)
{
    if (v1->i32 != 123 || v2->i32 != 456 || v3->i32 != 789) {
	fprintf(stderr, "ERROR: compiler is not obeying inline function directive!\n");
	fprintf(stderr, "       this program will not work correctly\n");
    printf("failed: %d %d %d\n", v1->i32, v2->i32, v3->i32);
	exit(1);
    }
    printf("passed: %d %d %d\n", v1->i32, v2->i32, v3->i32);
}

void
var_init(void)
{
    test_inline(int32s(123), int32s(456), int32s(789));
}

#ifdef VAR_MAIN
int
main(int argc, char **argv)
{
    char buf[BUFSIZ];
    var_t *var;

    var_init();

    var = int32s(666);

    var_to_str(int32s(225), buf, sizeof(buf));
    printf("int32 \"225\" = \"%s\"\n", buf);

    var_to_str(floats(12345.67890), buf, sizeof(buf));
    printf("float \"12345.67890\" = \"%s\"\n", buf);

    var_to_str(strs("Hello World!"), buf, sizeof(buf));
    printf("string \"Hello World!\" = \"%s\"\n", buf);

    var_to_str(char8s('A'), buf, sizeof(buf));
    printf("char \"A\" = \"%s\"\n", buf);

    var_to_str(bools(true), buf, sizeof(buf));
    printf("boolean \"true\" = \"%s\"\n", buf);

    return 0;
}
#endif
