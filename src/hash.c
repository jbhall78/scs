#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <assert.h>
#include <errno.h>
#include <arpa/inet.h>	/* htonl - probably should write my own implementation */

#include "mem.h"
#include "error.h"
#include "slist.h"
#include "mathlib.h"
#include "filelib.h"
#include "strlib.h"
#include "hash.h"

static hash_bucket_t *
hash_bucket_new(void)
{
    hash_bucket_t *bucket;

    bucket = malloc(sizeof(hash_bucket_t));
    memset(bucket, 0, sizeof(hash_bucket_t));

    return bucket;
}

/*
 * get the hash bucket the key resides in, if the create flag is true, create it if it does not exist, otherwise return NULL
 */
static hash_bucket_t *
hash_bucket_get(hash_table_t *table, void *_key, int key_len, boolean create)
{
    char *key = _key;
    int i;
    unsigned char c;
    hash_bucket_t *bp;
    int depth = MIN(key_len, table->max_depth);

    assert(table != NULL);
    assert(key != NULL);
    assert(key_len > 0);

    for (i = 0, bp = table->bucket; i < depth; i++) {
	c = key[i];

	if (bp->bytes[(unsigned int)c] == NULL) {
	    if (create) {
		bp->bytes[(unsigned int)c] = hash_bucket_new();
		bp->bytes[(unsigned int)c]->depth = bp->depth + 1;
	    } else {
		return NULL;
	    }
	}

	bp = bp->bytes[(int)c];
    }

    return bp;
}

/* get the existing hash bucket for the key, if it does not exist return NULL */
hash_table_t *
hash_table_new(void (*key_free)(void *), void (*val_free)(void *))
{
    hash_table_t *table;

    table = malloc(sizeof(hash_table_t));
    memset(table, 0, sizeof(hash_table_t));

    table->bucket = hash_bucket_new();
    table->max_depth = HASH_MAX_DEPTH;
    table->key_free = key_free;
    table->val_free = val_free;

    return table;
}

static hash_keypair_t *
hash_keypair_new(void *key, int key_len, void *val, int val_len)
{
    hash_keypair_t *keypair;

    keypair = mem_new0(hash_keypair_t);

    keypair->key = key;
    keypair->key_len = key_len;
    keypair->val = val;
    keypair->val_len = val_len;

    return keypair;
}

void
hash_set(hash_table_t *table, void *_key, int key_len, void *val, int val_len)
{
    slist_t *item;
    hash_keypair_t *keypair;
    hash_bucket_t *bp;

    bp = hash_bucket_get(table, _key, key_len, TRUE);
    assert(bp != NULL);

    for (item = bp->keypairs; item; item = item->next) {
	keypair = item->data;

	if (keypair->key_len != key_len)
	    continue;

	// duplicate key, overwrite
	if (memcmp(_key, keypair->key, key_len) == 0) {
	    if (keypair->val != val &&
		    table->val_free != NULL)
		table->val_free(keypair->val);
	    keypair->val = val;
	    keypair->val_len = val_len;
	    return;
	}
    }

    keypair = hash_keypair_new(_key, key_len, val, val_len);
    item = slist_new();
    item->data = keypair;

    // set value
    bp->keypairs = slist_add_start(bp->keypairs, item);
    table->items++;
}

boolean
hash_delete(hash_table_t *table, void *_key, int key_len)
{
    slist_t *item;
    hash_keypair_t *keypair;
    hash_bucket_t *bp;

    assert(table != NULL);
    assert(_key != NULL);
    assert(key_len > 0);

    bp = hash_bucket_get(table, _key, key_len, FALSE);
    if (bp == NULL)
	return false;

    for (item = bp->keypairs; item; item = item->next) {
	keypair = item->data;

	if (keypair->key_len != key_len)
	    continue;

	if (memcmp(_key, keypair->key, key_len) == 0) {
	    // found key, delete key, value and keypair
	    if (table->val_free != NULL)
		table->val_free(keypair->val);
	    if (table->key_free != NULL)
		table->key_free(keypair->key);
	    bp->keypairs = slist_remove(bp->keypairs, item, &mem_free);
	    return true;
	}
    }

    return false;
}

void
_hash_get(hash_table_t *table, void *_key, int key_len, void **val, int *val_len)
{
    hash_bucket_t *bp;
    slist_t *item;
    hash_keypair_t *keypair;

    assert(table != NULL);
    assert(_key != NULL);
    assert(key_len > 0);

    if (val == NULL && val_len == NULL)
	return; /* don't be stupid */

    if (val != NULL)
	*val = NULL;
    if (val_len != NULL)
	*val_len = 0;

    bp = hash_bucket_get(table, _key, key_len, FALSE);
    if (bp == NULL)
	return;

    for (item = bp->keypairs; item; item = item->next) {
	keypair = item->data;

	if (keypair->key_len != key_len)
	    continue;

	if (memcmp(_key, keypair->key, key_len) == 0) {
	    // found
	    if (val != NULL)
		*val = keypair->val;
	    if (val_len != NULL)
		*val_len = keypair->val_len;
	    return;
	}
    }

    return;
}

void
hash_table_set_max_depth(hash_table_t *table, uint32_t depth)
{
    table->max_depth = depth;

    // TODO: update existing items
}

void
hash_func(
	hash_table_t *table,
	hash_bucket_t *bucket,
	void (*callback)(hash_keypair_t *keypair, void *data1, void *data2), void *data1, void *data2)
{
    slist_t *item;
    hash_keypair_t *keypair;
    int i;

    assert(table != NULL);
    assert(bucket != NULL);
    assert(callback != NULL);

    for (item = bucket->keypairs; item; item = item->next) {
	keypair = item->data;
//	printf("%d [%s] = [%s]\n", bucket->depth, keypair->key, keypair->val);
	callback(keypair, data1, data2);
    }

    for (i = 0; i <= 255; i++) {
	if (bucket->bytes[i] != NULL)
	    hash_func(table, bucket->bytes[i], callback, data1, data2);
    }
}

static void
_hash_get_keys_array_hash_func(hash_keypair_t *keypair, void *data1, void *data2)
{
    void **array = data1;
    int *idx = data2;

    array[*idx] = keypair->key;
    //printf("index: %d\n", *idx);
    *idx = (*idx + 1);
}

void **
hash_get_keys_array(hash_table_t *table, int *items)
{
    void **array;
    int idx = 0;
    int size = sizeof(void *) * table->items;

    array = malloc(size);
    memset(array, 0, size);

    hash_func(table, table->bucket, _hash_get_keys_array_hash_func, array, &idx);

    *items = idx;

    return array;
}

static void
_hash_get_vals_array_hash_func(hash_keypair_t *keypair, void *data1, void *data2)
{
    void **array = data1;
    int *idx = data2;

    array[*idx] = keypair->val;
    *idx = (*idx + 1);
}

void **
hash_get_vals_array(hash_table_t *table, int *items)
{
    void **array;
    int idx = 0;
    int size = sizeof(void *) * table->items;

    array = malloc(size);
    memset(array, 0, size);

    hash_func(table, table->bucket, _hash_get_vals_array_hash_func, array, &idx);

    *items = idx;

    return array;
}

void
hash_load(hash_table_t *hash, char *filename)
{
    file_t *file;
    uint32_t len;
    string_t *key, *val;
    int ret;
    err_t *tmp = NULL;

    file = file_new(filename);
    ret = file_open(file, FILE_READ, &tmp);
    if (ret == false) {
	fprintf(stderr, "warning: hash_load: could not open file for reading: %s: %s\n", filename, tmp->msg);
	error_free(tmp);
	return;
    }

    key = str_new(NULL, 0);
    val = str_new(NULL, 0);

    while ((ret = file_read(file, &len, sizeof(uint32_t), NULL)) > 0) {
	/* we are misusing the string API here... */
	str_alloc(key, len + 1);
	if ((ret = file_read(file, key->txt, len, NULL)) != len) {
	    fprintf(stderr, "unexpected EOF or short read while reading key: %s\n", file->name);
	    exit(1);
	}
	key->len = len;
	key->txt[len] = 0;

	if ((ret = file_read(file, &len, sizeof(uint32_t), NULL)) != sizeof(uint32_t)) {
	    fprintf(stderr, "unexpected EOF or short read while reading value size: %s\n", file->name);
	    exit(1);
	}

	str_alloc(val, len + 1);
	if ((ret = file_read(file, val->txt, len, NULL)) != len) {
	    fprintf(stderr, "unexpected EOF or short read while reading value: %s\n", file->name);
	    exit(1);
	}
	val->len = len;
	val->txt[len] = 0;

	hash_set(hash, key->txt, key->len, val->txt, val->len);
    }

    file_close(file);
}

static void
_hash_save_func(hash_keypair_t *keypair, void *data1, void *data2)
{
    file_t *file = data1;
    uint32_t len;

    assert(file != NULL);

    len = htonl(keypair->key_len);
    file_write(file, &len, sizeof(uint32_t), NULL);
    file_write(file, keypair->key, keypair->key_len, NULL);

    len = htonl(keypair->val_len);
    file_write(file, &len, sizeof(uint32_t), NULL);
    file_write(file, keypair->val, keypair->val_len, NULL);
}

void
hash_save(hash_table_t *hash, char *filename)
{
    file_t *file;
    int ret;
    err_t *tmp = NULL;

    file = file_new(filename);
    ret = file_open(file, FILE_WRITE, &tmp);

    if (ret == false) {
	fprintf(stderr, "hash_save: could not open file for writing: %s: %s\n", filename, tmp->msg);
	error_free(tmp);
	exit(1);
    }

    hash_func(hash, hash->bucket, _hash_save_func, file, NULL);

    file_close(file);
}

static void
_hash_print_func(hash_keypair_t *keypair, void *data1, void *data2)
{
    //printf("[%s:%d] = [%s:%d]\n", (char *)keypair->key, keypair->key_len, (char *)keypair->val, keypair->val_len);
    //printf("[%s:%d] = [%d]\n", (char *)keypair->key, keypair->key_len, *((int *)keypair->val));
    printf("[%s:%d] = [%s]\n", (char *)keypair->key, keypair->key_len, (char *)keypair->val);
}

void
hash_print(hash_table_t *table)
{
    hash_func(table, table->bucket, _hash_print_func, NULL, NULL);
}

void
_hash_destroy(hash_table_t *table, hash_bucket_t *bucket)
{
    slist_t *item;
    hash_keypair_t *keypair;
    int i;

    assert(table != NULL);
    assert(bucket != NULL);

    // recurse into children first
    for (i = 0; i <= 255; i++) {
	if (bucket->bytes[i] != NULL)
	    _hash_destroy(table, bucket->bytes[i]);
    }

    // free bucket data
    for (item = bucket->keypairs; item; item = item->next) {
	keypair = item->data;
	table->key_free(keypair->key);
	table->val_free(keypair->val);
	free(keypair);
    }

    // free bucket
    free(bucket);
}

void
hash_destroy(hash_table_t *table)
{
    _hash_destroy(table, table->bucket);

    free(table);
}

#ifdef HASH_MAIN
int
main(int argc, char **argv)
{
    char *keys[] = { "abc", "def", "ghi", "jkl", "mno", "pqr", "stu", "vwx", "vwx" };
    char *vals[] = { "123", "456", "789", "012", "345", "678", "901", "234", "567" };
    char *val;
    int i, items;
    hash_table_t *table;
    char **rkeys;

    table = hash_new(&free, &free);

    for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
	hash_set(table, strdup(keys[i]), strlen(keys[i]), strdup(vals[i]), strlen(vals[i]));
    }

    printf("items: %d\n", table->items);
    hash_print(table);

    for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
	//val = hash_get(table, keys[i], strlen(keys[i]));
	hash_get(table, keys[i], strlen(keys[i]), (void **)&val, NULL);
	printf("%s = %s\n", keys[i], val);
    }

    rkeys = (char **)hash_get_keys_array(table, &items);
    printf("items: %d\n", items);
    for (i = 0; i < items; i++) {
	printf("key: %s\n", rkeys[i]);
    }
    free(rkeys);

    return 0;
}
#endif
