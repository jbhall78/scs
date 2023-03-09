#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "mem.h"
#include "dbuf.h"

bool
dbuf_read_bool(dbuf_t *dbuf, int8_t *res)
{
    int8_t n;

    if (dbuf->pos+1 > dbuf->len) {
	return false;
    }

    n = get8(dbuf->data, dbuf->pos);
    dbuf->pos += 1;

    *res = n;

    return true;
}

bool
dbuf_read_char8(dbuf_t *dbuf, int8_t *res)
{
    int8_t n;

    if (dbuf->pos+1 > dbuf->len) {
	return false;
    }

    n = get8(dbuf->data, dbuf->pos);
    dbuf->pos += 1;

    *res = n;

    return true;
}

bool
dbuf_read_uchar8(dbuf_t *dbuf, uint8_t *res)
{
    int8_t n;

    if (dbuf->pos+1 > dbuf->len) {
	return false;
    }

    n = get8(dbuf->data, dbuf->pos);
    dbuf->pos += 1;

    *res = n;

    return true;
}

bool
dbuf_read_int8(dbuf_t *dbuf, int8_t *res)
{
    int8_t n;

    if (dbuf->pos+1 > dbuf->len) {
	return false;
    }

    n = get8(dbuf->data, dbuf->pos);
    dbuf->pos += 1;

    *res = n;

    return true;
}

bool
dbuf_read_int16(dbuf_t *dbuf, int16_t *res)
{
    int16_t n;

    if (dbuf->pos+2 > dbuf->len)
	return false;

    n = get16(dbuf->data, dbuf->pos);
    dbuf->pos += 2;

    *res = n;

    return true;
}

bool
dbuf_read_int32(dbuf_t *dbuf, int32_t *res)
{
    int32_t n;

    if (dbuf->pos+4 > dbuf->len)
	return false;

    n = get32(dbuf->data, dbuf->pos);
    dbuf->pos += 4;

    *res = n;

    return true;
}

bool
dbuf_read_int64(dbuf_t *dbuf, int64_t *res)
{
    int64_t n;

    if (dbuf->pos+8 > dbuf->len)
	return false;

    n = get64(dbuf->data, dbuf->pos);
    dbuf->pos += 8;

    *res = n;

    return true;
}

bool
dbuf_read_uint8(dbuf_t *dbuf, uint8_t *res)
{
    uint8_t n;

    if (dbuf->pos+1 > dbuf->len) {
	return false;
    }

    n = get8(dbuf->data, dbuf->pos);
    dbuf->pos += 1;

    *res = n;

    return true;
}

bool
dbuf_read_uint16(dbuf_t *dbuf, uint16_t *res)
{
    uint16_t n;

    if (dbuf->pos+2 > dbuf->len)
	return false;

    n = get16(dbuf->data, dbuf->pos);
    dbuf->pos += 2;

    *res = n;

    return true;
}

bool
dbuf_read_uint32(dbuf_t *dbuf, uint32_t *res)
{
    uint32_t n;

    if (dbuf->pos+4 > dbuf->len)
	return false;

    n = get32(dbuf->data, dbuf->pos);
    dbuf->pos += 4;

    *res = n;

    return true;
}

bool
dbuf_read_uint64(dbuf_t *dbuf, uint64_t *res)
{
    uint64_t n;

    if (dbuf->pos+8 > dbuf->len)
	return false;

    n = get64(dbuf->data, dbuf->pos);
    dbuf->pos += 8;

    *res = n;

    return true;
}

bool
dbuf_read_float(dbuf_t *dbuf, float *res)
{
    float n;

    if (dbuf->pos+4 > dbuf->len)
	return false;

    n = get_float(dbuf->data, dbuf->pos);
    dbuf->pos += 4;

    *res = n;

    return true;
}

bool
dbuf_read_double(dbuf_t *dbuf, double *res)
{
    double n;

    if (dbuf->pos+8 > dbuf->len)
	return false;

    n = get_double(dbuf->data, dbuf->pos);
    dbuf->pos += 8;

    *res = n;

    return true;
}

bool
dbuf_read_size(dbuf_t *dbuf, void *dest, uint32_t len)
{
    if (dbuf->pos + len > dbuf->len)
	return false;

    memcpy(dest, dbuf->data + dbuf->pos, len);
    dbuf->pos += len;

    return true;
}

bool
dbuf_write_bool(dbuf_t *dbuf, int8_t n)
{
    if (dbuf->pos+1 > dbuf->len)
	return false;

    set8(dbuf->data, dbuf->pos, n);
    dbuf->pos += 1;

    return true;
}

bool
dbuf_write_char8(dbuf_t *dbuf, int8_t n)
{
    if (dbuf->pos+1 > dbuf->len)
	return false;

    set8(dbuf->data, dbuf->pos, n);
    dbuf->pos += 1;

    return true;
}

bool
dbuf_write_uchar8(dbuf_t *dbuf, uint8_t n)
{
    if (dbuf->pos+1 > dbuf->len)
	return false;

    set8(dbuf->data, dbuf->pos, n);
    dbuf->pos += 1;

    return true;
}

bool
dbuf_write_int8(dbuf_t *dbuf, int8_t n)
{
    if (dbuf->pos+1 > dbuf->len)
	return false;

    set8(dbuf->data, dbuf->pos, n);
    dbuf->pos += 1;

    return true;
}

bool
dbuf_write_int16(dbuf_t *dbuf, int16_t n)
{
    if (dbuf->pos+2 > dbuf->len)
	return false;

    set16(dbuf->data, dbuf->pos, n);
    dbuf->pos += 2;

    return true;
}

bool
dbuf_write_int32(dbuf_t *dbuf, int32_t n)
{
    if (dbuf->pos+4 > dbuf->len)
	return false;

    set32(dbuf->data, dbuf->pos, n);
    dbuf->pos += 4;

    return true;
}

bool
dbuf_write_int64(dbuf_t *dbuf, int64_t n)
{
    if (dbuf->pos+8 > dbuf->len)
	return false;

    set64(dbuf->data, dbuf->pos, n);
    dbuf->pos += 8;

    return true;
}

bool
dbuf_write_uint8(dbuf_t *dbuf, uint8_t n)
{
    if (dbuf->pos+1 > dbuf->len)
	return false;

    set8(dbuf->data, dbuf->pos, n);
    dbuf->pos += 1;

    return true;
}

bool
dbuf_write_uint16(dbuf_t *dbuf, uint16_t n)
{
    if (dbuf->pos+2 > dbuf->len)
	return false;

    set16(dbuf->data, dbuf->pos, n);
    dbuf->pos += 2;

    return true;
}

bool
dbuf_write_uint32(dbuf_t *dbuf, uint32_t n)
{
    if (dbuf->pos+4 > dbuf->len)
	return false;

    set32(dbuf->data, dbuf->pos, n);
    dbuf->pos += 4;

    return true;
}

bool
dbuf_write_uint64(dbuf_t *dbuf, uint64_t n)
{
    if (dbuf->pos+8 > dbuf->len)
	return false;

    set64(dbuf->data, dbuf->pos, n);
    dbuf->pos += 8;

    return true;
}

bool
dbuf_write_float(dbuf_t *dbuf, float n)
{
//    float *fptr;

    if (dbuf->pos+4 > dbuf->len)
	return false;

    set_float(dbuf->data, dbuf->pos, n);

/*
    fptr = (float *)(dbuf->data + dbuf->pos);
    printf("grr: %f %f\n", *fptr, n);
    assert(*fptr == n);
*/

    dbuf->pos += 4;

    return true;
}

bool
dbuf_write_double(dbuf_t *dbuf, double n)
{
    if (dbuf->pos+8 > dbuf->len)
	return false;

    set_double(dbuf->data, dbuf->pos, n);
    dbuf->pos += 8;

    return true;
}

bool
dbuf_write_size(dbuf_t *dbuf, void *src, uint32_t len)
{
    if (dbuf->pos + len > dbuf->len)
	return false;

    memcpy(dbuf->data + dbuf->pos, src, len);
    dbuf->pos += len;

    return true;
}

bool
dbuf_set_pos(dbuf_t *dbuf, uint32_t n)
{
    if (n >= dbuf->len) {
	dbuf->pos = dbuf->len - 1;
	return false;
    }

    dbuf->pos = n;

    return true;
}

/*
 * create a duplicate of the buffer & the data it holds
 */
dbuf_t *
dbuf_dup(dbuf_t *dbuf)
{
    dbuf_t *dbuf_new = mem_new0(dbuf_t);
    dbuf_new->len = dbuf->len;
    dbuf_new->alloc = dbuf->alloc;
    dbuf_new->free = true;

    dbuf_new->data = mem_malloc(dbuf->alloc);
    memcpy(dbuf_new->data, dbuf->data, dbuf->alloc);

    return dbuf_new;
}

dbuf_t *
dbuf_new(void *buf, int len)
{
    dbuf_t *dbuf = mem_new0(dbuf_t);

    if (buf == NULL && len > 0) {
	dbuf->data = mem_malloc0(len);
	dbuf->alloc = len;
	dbuf->len = len;
	dbuf->free = true;
    } else {
	dbuf->data = buf;
	dbuf->len = len;
    }

    return dbuf;
}

void
dbuf_destroy(dbuf_t *dbuf)
{
    if (dbuf->free)
	mem_free(dbuf->data);
    mem_free(dbuf);
}

#ifdef DBUF_MAIN
int
main(int argc, char **argv)
{
    char buf[BUFSIZ];

    return 0;
}
#endif
