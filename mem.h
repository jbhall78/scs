#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#define mem_new(t)	mem_alloc(sizeof(t))
#define mem_new0(t)	mem_alloc0(sizeof(t))

void mem_init(void);
void mem_print(void);
#define mem_malloc(x) mem_alloc(x)
#define mem_malloc0(x) mem_alloc0(x)
void *mem_alloc(size_t sz);
void *mem_alloc0(size_t sz);
void mem_free(void *ptr);
#define mem_freev(a) _mem_freev((void **)a)
#define mem_freevn(a,n) _mem_freevn((void **)a,n)
void _mem_freev(void **array);
void _mem_freevn(void **array, int n);

#endif
