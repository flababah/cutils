/*
 *    Copyright (c) 2013 Anders Høst
 */

#ifndef _LLIST_H_
#define _LLIST_H_

/*
 * Basic dual linked list and various operationers.
 */

#include <assert.h>

struct dllist {
	struct dllist *prev;
	struct dllist *next;
};

#define __container_of(ptr, T, member) \
		(T *)((void *)(ptr) - offsetof(T, member))

#define dllist_next(ptr, member) \
		__container_of(ptr->member.next, __typeof__(*ptr), member)

#define dllist_prev(ptr, member) \
		__container_of(ptr->member.prev, __typeof__(*ptr), member)

#define dllist_next_type(ptr, type, member) \
		__container_of((ptr)->next, type, member)

#define dllist_prev_type(ptr, type, member) \
		__container_of((ptr)->prev, type, member)

// Forward iteration excluding the head element.
#define for_dllist_iter(ptr, head_ptr, member) \
		for (ptr = dllist_next_type((head_ptr), __typeof__(*ptr), member); \
		     &ptr->member != (head_ptr); \
		     ptr = dllist_next(ptr, member))

// Reverse iteration excluding the head element.
#define for_dllist_iter_reverse(ptr, head_ptr, member) \
		for (ptr = dllist_prev_type((head_ptr), __typeof__(*ptr), member); \
		     &ptr->member != (head_ptr); \
		     ptr = dllist_prev(ptr, member))

static inline void __assert_dllist(struct dllist *list)
{
	assert(list->next);
	assert(list->prev);
}

static inline int dllist_empty(struct dllist *list)
{
	__assert_dllist(list);
	return list->next == list;
}

static inline void dllist_init(struct dllist *list)
{
	list->prev = list;
	list->next = list;
}

static inline void dllist_attach_before(struct dllist *current,
		struct dllist *new)
{
	__assert_dllist(current);

	new->next = current;
	new->prev = current->prev;
	current->prev->next = new;
	current->prev = new;
}

static inline void dllist_detach(struct dllist *list)
{
	__assert_dllist(list);

	list->next->prev = list->prev;
	list->prev->next = list->next;
}

static inline void dllist_reattach(struct dllist *list)
{
	__assert_dllist(list);

	list->next->prev = list;
	list->prev->next = list;
}

#endif // _LLIST_H_

