#ifndef _DBUF_H
#define _DBUF_H

#include <stdint.h>
#include <stdbool.h>

/* faster than memcpy() */
#define set8(buf, offset, val)  *((int8_t *)(((int8_t *)(buf) + (offset)))) = val
#define set16(buf, offset, val) *((int16_t *)(((int8_t *)(buf) + (offset)))) = val
#define set32(buf, offset, val) *((int32_t *)(((int8_t *)(buf) + (offset)))) = val
#define set64(buf, offset, val) *((int64_t *)(((int8_t *)(buf) + (offset)))) = val

#define set_float(buf, offset, val) *((float *)(((int8_t *)(buf) + (offset)))) = val
#define set_double(buf, offset, val) *((double *)(((int8_t *)(buf) + (offset)))) = val
  
#define get8(buf, offset)       *((int8_t *)(((int8_t *)(buf) + (offset))))
#define get16(buf, offset)      *((int16_t *)(((int8_t *)(buf) + (offset))))
#define get32(buf, offset)      *((int32_t *)(((int8_t *)(buf) + (offset))))
#define get64(buf, offset)      *((int64_t *)(((int8_t *)(buf) + (offset))))

#define get_float(buf, offset)      *((float *)(((int8_t *)(buf) + (offset))))
#define get_double(buf, offset)      *((double *)(((int8_t *)(buf) + (offset))))

#define dbuf_skip(buf, bytes)	dbuf_set_pos(buf, buf->pos + bytes);

// broken
//#define dbuf_dup(buf)	dbuf_new(buf->data, buf->len)


typedef struct {
    char *data;		// data bytes
    uint32_t pos;	// current position
    uint32_t len;	// length of buffer
    uint32_t alloc;	// allocated bytes
    bool free;		// free on destroy?
} dbuf_t;

bool dbuf_read_bool(dbuf_t *dbuf, int8_t *res);
bool dbuf_read_char8(dbuf_t *dbuf, int8_t *res);
bool dbuf_read_uchar8(dbuf_t *dbuf, uint8_t *res);
bool dbuf_read_int8(dbuf_t *dbuf, int8_t *res);
bool dbuf_read_int16(dbuf_t *dbuf, int16_t *res);
bool dbuf_read_int32(dbuf_t *dbuf, int32_t *res);
bool dbuf_read_int64(dbuf_t *dbuf, int64_t *res);
bool dbuf_read_uint8(dbuf_t *dbuf, uint8_t *res);
bool dbuf_read_uint16(dbuf_t *dbuf, uint16_t *res);
bool dbuf_read_uint32(dbuf_t *dbuf, uint32_t *res);
bool dbuf_read_uint64(dbuf_t *dbuf, uint64_t *res);
bool dbuf_read_float(dbuf_t *dbuf, float *res);
bool dbuf_read_double(dbuf_t *dbuf, double *res);
bool dbuf_read_size(dbuf_t *dbuf, void *dest, uint32_t len);
bool dbuf_write_bool(dbuf_t *dbuf, int8_t n);
bool dbuf_write_char8(dbuf_t *dbuf, int8_t n);
bool dbuf_write_uchar8(dbuf_t *dbuf, uint8_t n);
bool dbuf_write_int8(dbuf_t *dbuf, int8_t n);
bool dbuf_write_int16(dbuf_t *dbuf, int16_t n);
bool dbuf_write_int32(dbuf_t *dbuf, int32_t n);
bool dbuf_write_int64(dbuf_t *dbuf, int64_t n);
bool dbuf_write_uint8(dbuf_t *dbuf, uint8_t n);
bool dbuf_write_uint16(dbuf_t *dbuf, uint16_t n);
bool dbuf_write_uint32(dbuf_t *dbuf, uint32_t n);
bool dbuf_write_uint64(dbuf_t *dbuf, uint64_t n);
bool dbuf_write_float(dbuf_t *dbuf, float n);
bool dbuf_write_double(dbuf_t *dbuf, double n);
bool dbuf_write_size(dbuf_t *dbuf, void *src, uint32_t len);
bool dbuf_set_pos(dbuf_t *dbuf, uint32_t n);
dbuf_t *dbuf_dup(dbuf_t *dbuf);
dbuf_t *dbuf_new(void *buf, int len);
void dbuf_destroy(dbuf_t *dbuf);

#endif
