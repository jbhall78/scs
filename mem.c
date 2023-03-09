#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mem.h"

#undef DEBUG_ALLOCATIONS

//#define MAX_ALLOCATIONS 65530
#define MAX_ALLOCATIONS 655300

//typedef uint64_t allocation_t;
typedef uint32_t allocation_t;

static allocation_t allocations[MAX_ALLOCATIONS];

void
mem_init(void)
{
    memset(allocations, 0, MAX_ALLOCATIONS);
}

void
mem_print(void)
{

#ifdef DEBUG_ALLOCATIONS
    int i;
    printf("dumping allocations:\n");
    for (i = 0; i < MAX_ALLOCATIONS; i++) {
	if (allocations[i] != 0) 
	    printf("%016X\n", allocations[i]);
    }
#endif
}

void *
mem_alloc(size_t sz)
{
	void *ptr;

//	printf("trying to alloc ptr of size: %d ", sz);
	ptr = malloc(sz);

#ifdef DEBUG_ALLOCATIONS
	// XXX - need to make this thread safe
	int i;
	for (i = 0; i < MAX_ALLOCATIONS; i++) {
		if (allocations[i] == 0) {
			allocations[i] = (allocation_t)ptr;
			break;
		}
	}
//	printf("got: [%d] 0x%016X\n", i, (allocation_t)ptr);
#endif
	if (sz > 10000)
	    printf("allocated %d bytes: 0x%08x to 0x%08x\n", sz, (uint32_t)ptr, (uint32_t)ptr + sz);

	return ptr;
}

void *
mem_alloc0(size_t sz)
{
    void *ptr;

    ptr = mem_alloc(sz);

    memset(ptr, 0, sz);

    return ptr;
}

void
mem_free(void *ptr)
{

//	printf("trying to free ptr: 0x%016X\n", ptr);

#ifdef DEBUG_ALLOCATIONS
	int i;
	for (i = 0; i < MAX_ALLOCATIONS; i++) {
		if (allocations[i] == (allocation_t)ptr)
			allocations[i] = 0;
	}
#endif

	free(ptr);
}

void
_mem_freev(void **array)
{
    int i;
    for (i = 0; array[i] != NULL; i++)
	mem_free(array[i]);
    mem_free(array);
}

void
_mem_freevn(void **array, int n)
{
    int i;
    for (i = 0; i < n; i++)
	mem_free(array[i]);
    mem_free(array);
}
