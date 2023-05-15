#ifndef FS_HASH_H
#define FS_HASH_H

#include <stdint.h>

#include "slist.h"
#include "hash.h"

#define FS_HASH_MAX_DEPTH 10
#define FS_HASH_MEM_HASH_MAX_ITEMS 10000

typedef struct fs_hash_s {
    char *root;			// root directory
    hash_t *mem_hash;		// memory buffer
    uint32_t mem_hash_max_items;	// maximum items stored in memory hash
//    uint32_t items;		// number of items in fs_hash (uncalculated currently)
    uint32_t max_depth;		// maximum depth for fs_hash
} fs_hash_t;

boolean fs_hash_is_valid_key_char(char c);
boolean fs_hash_is_valid_key(char *key);
void fs_hash_hashify_key(fs_hash_t *fshash, char *key, char **hkey, char **hdir);
void fs_hash_set(fs_hash_t *fshash, char *key, void *val, int val_len);
void fs_hash_get(fs_hash_t *fshash, char *key, void **val, int *val_len);
void fs_hash_set_max_depth(fs_hash_t *table, uint32_t depth);
fs_hash_t *fs_hash_new(char *dir);


#endif
