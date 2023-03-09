#ifndef _DLIST_H
#define _DLIST_H

typedef struct dlist_s {
    void *data;
    struct dlist_s *next;
    struct dlist_s *prev;
} dlist_t;

dlist_t *dlist_new(void);
dlist_t *dlist_append(dlist_t *head, dlist_t *add);

dlist_t *dlist_add_start(dlist_t *head, dlist_t *item);
#define dlist_add_end dlist_append

#if 0
dlist_t *dlist_insert_before(dlist_t *head, dlist_t *add, dlist_t *before);
dlist_t *dlist_remove(dlist_t *head, dlist_t *del, void (*free_data_cb)(void *));
dlist_t *dlist_sort(dlist_t *head, int (*cmp)(dlist_t *,dlist_t *,void *), void *data);
dlist_t *dlist_index(dlist_t *head, unsigned int idx);
void dlist_destroy(dlist_t *head, void (*free_data_cb)(void *));
// only defined as char * to get around void * type limitations
// it is a contiguous array of pointers to the data pointers stored in the list

void **dlist_array(dlist_t *head);
#endif


#endif
