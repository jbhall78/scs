#ifndef VARRAY_H
#define VARRAY_H

typedef struct varray_s {
    void **data;
    // enum { ASCII, UNICODE, UTF8 } internal_format;
    void **iter;		// saved position during parsing functions
    size_t len;	// length of varray
    size_t alloc;	// allocated memory
    void (*free_cb)(void *);	// called to free data associated with pointer
} varray_t;

#define varray_eq(s1,s2) (s1->len == s2->len) ? (memcmp(s1->data, s2->data, s1->len * sizeof(void *)) == 0) : 0
#define varray_eqn(s1,s2,n) (s1->len >= n && s2->len >= n) ? (memcmp(s1->data, s2->data, n * sizeof(void *)) == 0) : 0
#define varray_len(s) s->len
#define varray_dup(a) varray_new(a->data, a->len)
#define varray_idx(va,i) va->data[i]

#define varray_new(d,l,cb)	_varray_new(d,l,(void (*)(void *))cb)
varray_t *_varray_new(void **data, size_t len, void (*free_cb)(void *));
void varray_free(varray_t *varray);
void varray_clear(varray_t *varray);
void varray_alloc(varray_t *varray, int size);
#define varray_push(va,d)	varray_append(va,d)
void varray_append(varray_t *varray, void *data);
void varray_append_varray(varray_t *varray, void **data, int count);

void varray_insert_at(varray_t *varray, void *data, int pos);
void varray_insert_varray_at(varray_t *varray, void **data, int count, int pos);
#define varray_pop(va)		varray_delete_at(va, 1, va->len-1)
void varray_delete_at(varray_t *varray, int len, int pos);
void varray_delete(varray_t *varray, void *data);
void varray_truncate_from(varray_t *varray, int pos);

//void varray_reset_iter(varray_t *varray);
//varray_t *varray_remainder(varray_t *varray);

#endif
