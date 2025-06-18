#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>
#include <unistd.h>

#include "mem.h"
#include "varray.h"

/*
 * Dynamically Sized Void Pointer Array
 */

#ifndef MIN
#define MIN(a,b) (a < b) ? a : b
#endif

#ifndef MAX
#define MAX(a,b) (a > b) ? a : b
#endif

void
varray_free_item(varray_t *varray, int i)
{
    if (varray->free_cb == NULL)
	return;

    varray->free_cb(varray->data[i]);

}

varray_t *
_varray_new(void **data, size_t count, void (*free_cb)(void *))
{
    varray_t *varray = mem_new0(varray_t);

    if (data != NULL && count > 0) {
	varray->data = mem_malloc((count+1) * sizeof(void *)); // NULL terminated
	varray->alloc = count+1;

	varray->data[count] = 0;

	memcpy(varray->data, data, count * sizeof(void *)); 
	varray->len = count;
    } else if (data == NULL && count == 0) {
	varray->data = mem_malloc(1);	// NULL terminated
	varray->alloc = 1;
	varray->data[0] = 0;
    }

    varray->iter = varray->data;
    varray->free_cb = free_cb;

    return varray;
}

void
varray_free(varray_t *varray)
{
    int i;

    for (i = 0; i < varray->len; i++)
	varray_free_item(varray, i);

    mem_free(varray->data);
    mem_free(varray);
}

void
varray_clear(varray_t *varray)
{
    int i;

    for (i = 0; i < varray->len; i++)
	varray_free_item(varray, i);

    varray->len = 0;
    varray->iter = varray->data;
//    memset(varray->data, 0, varray->alloc);
    varray->data[0] = 0; // faster
}

// ensure varray has alloc'd enough space for at least count elements
void
varray_alloc(varray_t *varray, int count)
{
    void **save;
    int pos;

    if (count >= varray->alloc) {
	pos = (varray->iter - varray->data) / sizeof(void *);
#ifdef DEBUG
	varray->alloc += MAX(1,count - varray->alloc);	// allocate at least 100 bytes // 1= force to use logic
#else
	varray->alloc += MAX(100,count - varray->alloc);	// allocate at least 100 bytes
#endif
	save = varray->data;
	varray->data = mem_malloc(varray->alloc * sizeof(void *));
	memcpy(varray->data, save, varray->len * sizeof(void *));
	varray->data[varray->len] = 0;
	mem_free(save);
	varray->iter = varray->data + (pos * sizeof(void *));
    }
}

/*
 * Append Functions
 */
void
varray_append(varray_t *varray, void *data)
{
    varray_alloc(varray, varray->len + 1);

    varray->data[varray->len] = data;
    varray->len++;
    varray->data[varray->len] = 0;
}

void
varray_append_varray(varray_t *varray, void **data, int len)
{
    void *p;

    varray_alloc(varray, varray->len + len + 1);

    p = &varray->data[varray->len];
    memcpy(p, data, len * sizeof(void *));
    varray->len += len;
    varray->data[varray->len] = 0;
}

/*
 * Insert Functions
 */
void
varray_insert_at(varray_t *varray, void *data, int pos)
{
    int i;

    if (pos < 0 || pos > varray->len)
	return;

    varray_alloc(varray, varray->len + 1);

    // shift end of varray by one
    for (i = varray->len+1; i > pos; i--)
	varray->data[i] = varray->data[i-1];

    // set item
    varray->data[pos] = data;

    // increase length
    varray->len++;
}

void
varray_insert_varray_at(varray_t *varray, void **data, int len, int pos)
{
    void **p1, **p2;
    int end_size;
    void **save;

    if (pos < 0 || pos > varray->len)
	return;

    varray_alloc(varray, varray->len + len);

    end_size = (varray->len - pos) * sizeof(void *);
    save = mem_malloc(end_size);
    p1 = &varray->data[pos];
    memcpy(save, p1, end_size);
    p2 = &varray->data[pos+len];
    memcpy(p2, save, end_size);
    mem_free(save);

    // insert varraying
    memcpy(p1, data, len * sizeof(void *));

    // increase length
    varray->len += len;
}

/*
 * Delete Functions
 */
void
varray_delete_at(varray_t *varray, int len, int pos)
{
    void **p1, **p2;
    void **save;
    int end_size;
    int i;

#if 1
    assert(pos >= 0);
    assert(pos < varray->len);
    assert(len <= varray->len);
#else
    if (pos < 0 || pos >= varray->len || len > varray->len)
	return;
#endif

    for (i = pos; i < len && i < varray->len; i++)
	varray_free_item(varray, i);

    end_size = (varray->len - (pos+len)) * sizeof(void *);
    save = mem_malloc(end_size);

    p1 = &varray->data[pos+len];
    memcpy(save, p1, end_size);

    p2 = &varray->data[pos];
    memcpy(p2, save, end_size);
    free(save);

    varray->len -= len;
    varray->data[varray->len] = 0;
}

void
varray_delete(varray_t *varray, void *data)
{
    int i;

    for (i = 0; i < varray->len; i++) {
	if (varray->data[i] == data)
	    varray_delete_at(varray, 1, i);
    }
}

void
varray_truncate_from(varray_t *varray, int pos)
{
    int i;

    if (pos < 0 || pos > varray->len)
	return;

    for (i = pos; i < varray->len; i++)
	varray_free_item(varray, i);

    varray->len = pos;
    varray->data[varray->len] = 0;
}

/*
 * Iterating Functions
 */
void
varray_reset_iter(varray_t *varray)
{
    varray->iter = varray->data;
}

void
varray_print(varray_t *va)
{
    int i;

    for (i = 0; i < va->len; i++) {
	printf("[%d] 0x%08lx (%s)\n", i, (uint64_t)va->data[i], (char *)va->data[i]);
	//printf("[%d] 0x%08x\n", i, va->data[i]);
    }
}

#ifdef VARRAY_MAIN
int
main(int argc, char **argv)
{
    varray_t *va;
    int start = 0x10001000;

    mem_init();

    va = varray_new(NULL, 0, NULL);
    varray_append(va, (void *)start++);
    varray_append(va, (void *)start++);
    varray_append(va, (void *)start++);

    varray_print(va);

    varray_delete_at(va, 1, va->len-1);

    varray_print(va);

    mem_print();

    return 0;
}
#endif
