/*
 * Name: net_pkt.c
 * Description: Functions for working with packets
 *
 * Copyright (c) 2012 Jason Hall <jbhall78@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "mem.h"
#include "dbuf.h"
#include "net.h"

void *
net_pkt_pack(pkt_t *pkt, uint32_t *len)
{
    // seek to beginning and write in header values
    dbuf_set_pos(pkt->dbuf, 0);

    // BUG: convert between byte orders
    dbuf_write_uint16(pkt->dbuf, pkt->type);
    dbuf_write_uint32(pkt->dbuf, pkt->session_id);
    dbuf_write_uint32(pkt->dbuf, pkt->sequence);
    dbuf_write_uint8(pkt->dbuf, pkt->reliable);

    *len = pkt->dbuf->len;

    return pkt->dbuf->data;
}

pkt_t *
net_pkt_unpack(void *buf, uint32_t len)
{
//    printf("net_pkt_unpack: %d %d\n", len, MAX_PACKET_SIZE);
    assert(len <= MAX_PACKET_SIZE);

    pkt_t *pkt = net_pkt_new(0, false);
    pkt->dbuf = dbuf_new(NULL, MAX_PACKET_SIZE);
    memcpy(pkt->dbuf->data, buf, len);

    // BUG: convert between byte orders
    dbuf_read_uint16(pkt->dbuf, &pkt->type);
    dbuf_read_uint32(pkt->dbuf, &pkt->session_id);
    dbuf_read_uint32(pkt->dbuf, &pkt->sequence);
    dbuf_read_uint8(pkt->dbuf, &pkt->reliable);
    dbuf_set_pos(pkt->dbuf, PKT_HEADER_SIZE);

    return pkt;
}

#if 0
void
net_pkt_pack_int32(pkt_t *pkt, uint16_t len, int32_t *val)
{
    int i;

    for (i = 0; i < len; i++)
	dbuf_write_int32(pkt->dbuf, val[i]);
}

void
net_pkt_pack_uint32(pkt_t *pkt, uint16_t len, uint32_t *val)
{
    int i;

    for (i = 0; i < len; i++)
	dbuf_write_uint32(pkt->dbuf, val[i]);
}

void
net_pkt_pack_real(pkt_t *pkt, uint16_t len, real *val)
{
    int i;

//    printf("pack: %f\n", val[0]);
    for (i = 0; i < len; i++) {
#ifdef MATH64
	dbuf_write_double(pkt->dbuf, val[i]);	
#else
	dbuf_write_float(pkt->dbuf, val[i]);	
#endif
    }
}

void
net_pkt_pack_str(pkt_t *pkt, uint16_t len, char *val)
{
//    printf("packing string@%d: [%d]\"%s\"\n", pkt->dbuf->pos, len, val);
    // BUG: check bounds
    dbuf_write_int16(pkt->dbuf, len);
    dbuf_write_size(pkt->dbuf, val, len);
}

void
net_pkt_unpack_int32(pkt_t *pkt, uint16_t count, int32_t *val)
{
    int i;

    for (i = 0; i < count; i++)
	dbuf_read_int32(pkt->dbuf, &val[i]);
}

void
net_pkt_unpack_uint32(pkt_t *pkt, uint16_t count, uint32_t *val)
{
    int i;

    for (i = 0; i < count; i++)
	dbuf_read_uint32(pkt->dbuf, &val[i]);
}

void
net_pkt_unpack_str(pkt_t *pkt, uint16_t *len, char **val)
{
    dbuf_read_uint16(pkt->dbuf, len);

    // BUG: check bounds
    char *str = mem_malloc0(*len);

    dbuf_read_size(pkt->dbuf, str, *len);

//    printf("unpacking string@%d: [%d]\"%s\"\n", pkt->dbuf->pos, *len, str);

    *val = str;
}

void
net_pkt_unpack_real(pkt_t *pkt, uint16_t count, real *val)
{
    int i;

    for (i = 0; i < count; i++)
#ifdef MATH64
	dbuf_read_double(pkt->dbuf, &val[i]);
#else
	dbuf_read_float(pkt->dbuf, &val[i]);
#endif
//    printf("unpack: %f\n", val[0]);
}
#endif

pkt_t *
net_pkt_new(uint16_t type, bool reliable)
{
    pkt_t *pkt = g_new0(pkt_t, 1);

    pkt->type = type;
    pkt->reliable = reliable;

    pkt->dbuf = dbuf_new(NULL, MAX_PACKET_SIZE);	// maximum packet size
    dbuf_set_pos(pkt->dbuf, PKT_HEADER_SIZE);

    return pkt;
}

pkt_t *
net_pkt_dup(net_state_t *net, pkt_t *pkt)
{
    pkt_t *pkt_new = mem_new0(pkt_t);

    memcpy(pkt_new, pkt, sizeof(pkt_t));
    pkt->dbuf = dbuf_dup(pkt->dbuf);
    dbuf_set_pos(pkt_new->dbuf, PKT_HEADER_SIZE);

    return pkt_new;
}

void
net_pkt_delete(pkt_t *pkt)
{
    dbuf_destroy(pkt->dbuf);
    mem_free(pkt);
}
