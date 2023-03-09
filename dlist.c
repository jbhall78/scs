#include <stdio.h>
#include <stdlib.h>


#include <assert.h>

#include "dlist.h"


//static dlist_t *head;


dlist_t *
dlist_new(void)
{
    dlist_t *item = malloc(sizeof(dlist_t));
    assert(item != NULL);
    item->data = NULL;
    item->next = NULL;
    item->prev = NULL;
    return item;
}

dlist_t *
dlist_append(dlist_t *head, dlist_t *add)
{
    dlist_t *item;

    if (head == NULL) {
	return add;
    }

    for (item = head; item; item = item->next) {
	if (item->next == NULL) {
	    item->next = add;
	    add->prev = item;
	    break;
	}
    }

    return head;
}

dlist_t *
dlist_add_start(dlist_t *head, dlist_t *item)
{
    if (head != NULL)
	head->prev = item;
    item->next = head;
    head = item;

    return head;
}

#if 0
dlist_t *
dlist_insert_before(dlist_t *head, dlist_t *add, dlist_t *before)
{
    dlist_t *item, *prev;

    for (item = head; item; item = item->next) {
	if (item == before) {
	    if (item == head) {
		add->next = head;
		head = add;
	    } else {
		prev->next = add;
		add->next = item;
	    }
	    break;
	}
	prev = item;
    }
    return head;
}


// remove from list WITHOUT free'ing data
dlist_t *
dlist_unlink(dlist_t *head, dlist_t *del)
{
    dlist_t *prev = NULL;
    dlist_t *item;

    for (item = head; item; item = item->next) {
	// check to see if this is the item to delete
	if (item != del) {
	    prev = item;
	    continue;
	}

	// if we are mid-dlist, update the next pointer on the previous item
	// thus removing the reference to the item to delete
	if (prev) {
	    prev->next = item->next;
	}

	// if the item to delete is at the head of the dlist, update the head
	// item's next pointer to be its child item removing the item from the
	// dlist
	// NOTE: this could be a "else if" instead of "if" as well for one less
	//       comparison in the loop
	if (item == head) {
	    head = head->next;
	}

	// removal completed
	break;
    }

    return head;
}

// remove from list WITH free'ing data
dlist_t *
dlist_remove(dlist_t *head, dlist_t *del, void (*free_data_cb)(void *))
{
    dlist_t *prev = NULL;
    dlist_t *item;

    for (item = head; item; item = item->next) {
	// check to see if this is the item to delete
	if (item != del) {
	    prev = item;
	    continue;
	}

	// if we are mid-dlist, update the next pointer on the previous item
	// thus removing the reference to the item to delete
	if (prev) {
	    prev->next = item->next;
	}

	// if the item to delete is at the head of the dlist, update the head
	// item's next pointer to be its child item removing the item from the
	// dlist
	// NOTE: this could be a "else if" instead of "if" as well for one less
	//       comparison in the loop
	if (item == head) {
	    head = head->next;
	}

	// removal completed, free memory associated with the item and exit loop
	if (free_data_cb)
	    free_data_cb(item->data);

	free(item);
	break;
    }

    return head;
}

dlist_t *
dlist_index(dlist_t *list, unsigned int idx)
{
    dlist_t *item;
    unsigned int i;

    i = 0;
    for (item = list; item; item = item->next) {
	if (i == idx)
	    return item;
	i++;
    }
    return NULL;
}

void
dlist_print(dlist_t *head)
{
    dlist_t *lp;
    int i;

    for (i = 0, lp = head; lp; lp = lp->next, i++) {
	printf("%d ", *((int *)lp->data));
    }
    printf("\n");
}


dlist_t *
dlist_sort(dlist_t *head, int (*cmp)(dlist_t *,dlist_t *,void *), void *data)
{
    dlist_t *A, *B;

start:
    for (A = head; A; A = A->next) {
	for (B = A->next; B; B = B->next) {
	    if (cmp(A, B, data) == -1) {
		head = dlist_unlink(head, B);
		head = dlist_insert_before(head, /*item*/B, /*before*/A);
		goto start;
	    }
	}
    }

    return head;
}

unsigned int
dlist_items(dlist_t *head)
{
    dlist_t *item;
    unsigned int i = 0;

    for (item = head; item; item = item->next)
	i++;

    return i;
}

void **
dlist_array(dlist_t *head)
{
    unsigned int n = dlist_items(head);
    struct empty_s **array;
    unsigned int i;
    dlist_t *item;

    array = malloc(n * sizeof(struct empty_s *) + 1);
    for (item = head, i = 0; item; item = item->next, i++)
	array[i] = (struct empty_s *)item->data;

    array[i] = NULL;

    return (void **)array;
}


void
dlist_destroy(dlist_t *head, void (*free_data_cb)(void *))
{
    dlist_t *item, *next;

    for (item = head; item; item = next) {
	next = item->next;
	if (free_data_cb)
	    free_data_cb(item->data);
	free(item);
    }
}
#endif

/*
 * returns: -1 for A greater
 *           0 for equal
 *           1 for B greater
 */
#if 0
int
cmp(dlist_t *a, dlist_t *b, void *data)
{
    int A = *((int *)a->data);
    int B = *((int *)b->data);

    if (A > B) {
	return -1;
    }
    if (B > A) {
	return 1;
    }
    return 0;
}
#endif

