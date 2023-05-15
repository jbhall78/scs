#ifndef _HASH_H
#define _HASH_H

#include <stdint.h>

#include "mathlib.h"
#include "slist.h"

#define HASH_MAX_DEPTH 10

#define hash_new(a,b) hash_table_new(a,b)

#ifndef MIN
#define MIN(a,b) (a < b) ? a : b
#endif

typedef struct hash_keypair_s {
    void *key;
    int key_len;
    void *val;
    int val_len;
} hash_keypair_t;

//
// hash bucket used for binary (8 bit) search
//
typedef struct hash_bucket_s {
    struct hash_bucket_s *bytes[256];	// pointers to buckets of first byte of key
    uint32_t depth;		// depth of current bucket
    slist_t *keypairs;
} hash_bucket_t;

typedef struct hash_table_table_s {
    hash_bucket_t	*bucket;
    // only set on toplevel
    uint32_t items;
    uint32_t max_depth;		// maximum depth for hash
    void (*key_free)(void *);
    void (*val_free)(void *);
} hash_table_t;

typedef hash_table_t hash_t;


hash_table_t *hash_table_new(void (*key_free)(void *), void (*val_free)(void *));
void hash_set(hash_table_t *table, void *_key, int key_len, void *val, int val_len);
//void *hash_get(hash_table_t *table, void *_key, int key_len);
#define hash_get(h,k,kl,v,vl)	_hash_get(h,k,kl,(void **)v,vl)
void _hash_get(hash_table_t *table, void *_key, int key_len, void **_val, int *val_len);
void hash_table_set_max_depth(hash_table_t *table, uint32_t depth);
void hash_func(
	hash_table_t *table,
	hash_bucket_t *bucket,
	void (*callback)(hash_keypair_t *keypair, void *data1, void *data2), void *data1, void *data2);
void **hash_get_keys_array(hash_table_t *table, int *items);
void **hash_get_vals_array(hash_table_t *table, int *items);
void hash_load(hash_table_t *hash, char *filename);
void hash_save(hash_table_t *hash, char *filename);
void hash_print(hash_table_t *table);
void hash_destroy(hash_table_t *table);
boolean hash_delete(hash_table_t *table, void *_key, int key_len);


#endif
