#ifndef ARRAY_H
#define ARRAY_H

typedef struct array_s {
    char *data;
    int dsize;
    // enum { ASCII, UNICODE, UTF8 } internal_format;
    char *iter;		// saved position during parsing functions
    size_t len;	// length of array
    size_t alloc;	// allocated memory
    void (*free_cb)(void *);	// called to free any associated data
} array_t;

#define array_eq(s1,s2) (s1->len == s2->len && s1->dsize == s2->dsize) ? (memcmp(s1->data, s2->data, s1->len * s1->dsize) == 0) : 0
#define array_eqn(s1,s2,n) (s1->len >= n && s2->len >= n && s1->dsize == s2->dsize) ? (memcmp(s1->data, s2->data, n * s1->dsize) == 0) : 0
#define array_len(s) s->len
#define array_dup(a) array_new(a->dsize, a->data, a->len, a->free_cb)
#define array_idx(a,i) (a->data + (a->dsize * (i)))
#define array_insert_at(a,d,p) array_insert_array_at(a,d,1,p)
array_t *_array_new(size_t dsize, void *data, size_t len, void (*free_cb)(void *));
#define array_new(t,d,l,f) _array_new(sizeof(t), d,l,f)

void array_cp(array_t *src, array_t *dst);

void array_free(array_t *array);
void array_clear(array_t *array);
void array_alloc(array_t *array, int size);
void array_append(array_t *array, void *data);
void array_append_array(array_t *array, void *data, int count);

void array_insert_array_at(array_t *array, void *data, int count, int pos);
void array_delete_at(array_t *array, int len, int pos);
void array_truncate_from(array_t *array, int pos);

//void array_reset_iter(array_t *array);
//array_t *array_remainder(array_t *array);

#endif
