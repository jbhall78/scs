#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <assert.h>

#include "mem.h"
#include "array.h"

/*
 * Dynamically Sized Void Pointer Array
 */

#ifndef MIN
#define MIN(a,b) (a < b) ? a : b
#endif

#ifndef MAX
#define MAX(a,b) (a > b) ? a : b
#endif

static void
array_terminate(array_t *array)
{
    void *p;
    int i;

    p = array->data + (array->dsize * array->len);
    for (i = 0; i < array->dsize; i++)
	*((char *)p + i) = 0;
}

static void
array_free_item(array_t *array, int i)
{
    void *p;

    if (array->free_cb == NULL)
	return;

    p = array->data + (array->dsize * i);
    array->free_cb(p);
}

array_t *
_array_new(size_t dsize, void *data, size_t count, void (*free_cb)(void *))
{
    array_t *array = mem_new0(array_t);

    assert(dsize > 0);

    array->dsize = dsize;

    if (data != NULL && count > 0) {
	array->data = mem_malloc((count+1) * array->dsize); // NULL array_terminated
	array->alloc = count+1;
	array->len = count;
	memcpy(array->data, data, array->len * array->dsize);
    } else if (data == NULL && count == 0) {
	array->data = mem_malloc(1 * array->dsize);	// NULL array_terminated
	array->alloc = 1;
    } else if (count > 0) {
	array->data = mem_malloc((count+1) * array->dsize);	// NULL array_terminated
	array->alloc = (count+1);
    }

    array->iter = array->data;
    array->free_cb = free_cb;
    array_terminate(array);

    return array;
}

void
array_free(array_t *array)
{
    int i;

    for (i = 0; i < array->len; i++)
	array_free_item(array, i);

    mem_free(array->data);
    mem_free(array);
}

void
array_clear(array_t *array)
{
    int i;

    for (i = 0; i < array->len; i++)
	array_free_item(array, i);

    array->len = 0;
    array->iter = array->data;
//    memset(array->data, 0, array->alloc);
    array_terminate(array);
}

// ensure array has alloc'd enough space for at least count elements
void
array_alloc(array_t *array, int count)
{
    void *save;
    int pos;

    if (count > array->alloc) {
	pos = (array->iter - array->data) / array->dsize;
#ifdef DEBUG
	array->alloc += MAX(1,count - array->alloc) + 1;	// allocate at least 100 items // 1= force to use logic
#else
	array->alloc += MAX(100,count - array->alloc) + 1;	// allocate at least 100 items
#endif
	save = array->data;
	printf("allocating: 0x%08lx %ld\n", array->alloc * array->dsize, array->alloc * array->dsize);
	array->data = mem_malloc(array->alloc * array->dsize);
	memcpy(array->data, save, array->len * array->dsize);
	array_terminate(array);
	mem_free(save);
	array->iter = array->data + (pos * array->dsize);
    }
}

void
array_cp(array_t *src, array_t *dst)
{
    void *data = dst->data;
    memcpy(dst, src, sizeof(array_t));
    dst->data = data;
    array_alloc(dst, src->alloc);
    memcpy(dst->data, src->data, src->len * src->dsize);
    array_terminate(dst);
}

/*
 * Append Functions
 */
void
array_append(array_t *array, void *data)
{
    void *p;

    array_alloc(array, array->len + 1);	// NULL array_terminated

    // append element
    p = array->data + (array->dsize * array->len);
    memcpy(p, data, array->dsize);
    array->len++;

    // zero element at end
    array_terminate(array);
}

void
array_append_array(array_t *array, void *data, int len)
{
    void *p;

    array_alloc(array, array->len + len + 1);

    p = array->data + (array->dsize * array->len);
    memcpy(p, data, len * array->dsize);
    array->len += len;

    // zero element at end
    array_terminate(array);
}

/*
 * Insert Functions
 */
#if 0
void
array_insert_at(array_t *array, void *data, int pos)
{
    int i;

    if (pos < 0 || pos > array->len)
	return;

    array_alloc(array, array->len + 1 + 1); // new item & NULL array_terminated

    // shift end of array by one
    for (i = array->len+1; i > pos; i--)
	array->data[i] = array->data[i-1];

    // set item
    array->data[pos] = data;

    // increase length
    array->len++;
}
#endif

void
array_insert_array_at(array_t *array, void *data, int len, int pos)
{
    void *p1, *p2;
    int end_size;
    void *save;

    if (pos < 0 || pos > array->len)
	return;

    array_alloc(array, array->len + len + 1);	// NULL array_terminated

    end_size = (array->len - pos) * array->dsize;
    save = mem_malloc(end_size);
    p1 = array->data + (array->dsize * pos);
    memcpy(save, p1, end_size);
    p2 = array->data + (array->dsize * (pos+len));
    memcpy(p2, save, end_size);
    mem_free(save);

    // insert arraying
    memcpy(p1, data, len * array->dsize);

    // increase length
    array->len += len;

    array_terminate(array);
}

/*
 * Delete Functions
 */
void
array_delete_at(array_t *array, int len, int pos)
{
    void *p1, *p2;
    void *save;
    int end_size;
    int i;

#if 1
    assert(pos >= 0);
    assert(pos < array->len);
    assert(len <= array->len);
#else
    if (pos < 0 || pos >= array->len || len > array->len)
        return;
#endif

    for (i = pos; i < len && i < array->len; i++)
	array_free_item(array, i);

    end_size = (array->len - (pos+len)) * sizeof(void *);
    save = mem_malloc(end_size);

    p1 = array->data + (array->dsize * (pos+len));
    memcpy(save, p1, end_size);

    p2 = array->data + (array->dsize * (pos));
    memcpy(p2, save, end_size);
    free(save);

    array->len -= len;

    array_terminate(array); 
}

void
array_truncate_from(array_t *array, int pos)
{
    int i;

    if (pos < 0 || pos > array->len)
	return;

    for (i = pos; i < array->len; i++)
	array_free_item(array, i);

    array->len = pos;

    array_terminate(array);
}

/*
 * Iterating Functions
 */
void
array_reset_iter(array_t *array)
{
    array->iter = array->data;
}

void
array_get(array_t *array, int i, void *p)
{
    assert(i >= 0);
    assert(i < array->len);

    memcpy(p, array->data + (i * array->dsize), array->dsize);
}

/*
void
array_print(array_t *va)
{
    int i;

    for (i = 0; i < va->len; i++) {
	printf("[%d] 0x%08x\n", i, va->data[i]);
    }
}
*/

#ifdef ARRAY_MAIN
int
main(int argc, char **argv)
{
    int i, j;
    array_t *a;
    struct foo {
	char bar[256];
    };
    char baz[256];

    mem_init();

    a = array_new(int, NULL, 0, NULL);
    i = 13;
    array_append(a, &i);

    for (i = 0; i < 4; i++) {
	array_insert_at(a, &i, 0);
    }

    array_delete_at(a, 1, 1);

    for (i = 0; i < a->len; i++) {
	array_get(a, i, &j);
	printf("[%d] %d\n", i, j);
    }

    mem_print();

    array_free(a);
    array_new(struct foo, NULL, 1024 * 768, NULL);

    for (i = 0; i < 1024; i++) {
	for (j = 0; j < 768; j++) {
	    struct foo *foo;

	    foo = array_idx(a, i * 768 + j);

	    if (foo > a->data + (a->dsize * a->alloc)) {
		fprintf(stderr, "wtf: 0x%08x 0x%08x (%d, %d)\n", foo, a->data + (a->dsize * a->alloc), a->dsize, a->alloc);
		exit(1);
	    }

	    memset(baz, (i * 1024 + j) % 255, 255);
	    baz[255] = 0;

	    memcpy(foo->bar, baz, 255);
	}
    }
    for (i = 0; i < 1024 * 768; i++) {
	struct foo *foo = array_idx(a, i);
	memcpy(baz, foo->bar, 255);
    }

    return 0;
}
#endif
