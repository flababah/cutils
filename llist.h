/*
 *    Copyright (c) 2013 Anders Høst
 *
 *    Basic dual linked lists and various operations.
 */

#ifndef _LLIST_H_
#define _LLIST_H_

#if !defined(__GNUC__) && !defined(__MINGW32__)
	#error "llist.h is only for GCC/Clang!"
#endif

#include <assert.h>

struct dllist {
	struct dllist *prev, *next;
};

static inline void __assert_dllist_not_null(struct dllist *link)
{
	assert(link->next); assert(link->prev);
}

static inline void __assert_dllist(struct dllist *link)
{
	__assert_dllist_not_null(link);

	assert(link->next->prev == link);
	assert(link->prev->next == link);
}

#ifndef offsetof
	#define offsetof __builtin_offsetof
#endif

#define __container_of(ptr, T, member) \
		((T *)((void *)(ptr) - offsetof(T, member)))

#define dllist_next(ptr, member) ({ \
		__assert_dllist(&(ptr)->member); \
		__container_of((ptr)->member.next, __typeof__(*(ptr)), member); })

#define dllist_prev(ptr, member) ({ \
		__assert_dllist(&(ptr)->member); \
		__container_of((ptr)->member.prev, __typeof__(*(ptr)), member); })

// If head is not located in the same type as ptr.
#define dllist_next_type(ptr, type, member) ({ \
		__assert_dllist(ptr); \
		__container_of((ptr)->next, type, member); })

#define dllist_prev_type(ptr, type, member) ({ \
		__assert_dllist(ptr); \
		__container_of((ptr)->prev, type, member); })

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

static inline int dllist_empty(struct dllist *list)
{
	__assert_dllist(list);

	return list->next == list;
}

static inline int dllist_length(struct dllist *list)
{
	struct dllist *ptr = list->next;
	int count = 0;

	while (ptr != list)
		ptr = ptr->next, count++;
	return count;
}

static inline void dllist_init(struct dllist *list)
{
	list->prev = list; list->next = list;
}

static inline void dllist_attach_before(struct dllist *current,
		struct dllist *new)
{
	__assert_dllist(current);

	new->next = current; new->prev = current->prev;
	current->prev->next = new; current->prev = new;
}

static inline void dllist_attach_after(struct dllist *current,
		struct dllist *new)
{
	__assert_dllist(current);

	new->prev = current; new->next = current->next;
	current->next->prev = new; current->next = new;
}

static inline void dllist_detach(struct dllist *list)
{
	__assert_dllist(list);

	list->next->prev = list->prev; list->prev->next = list->next;
}

static inline void dllist_reattach(struct dllist *list)
{
	__assert_dllist_not_null(list);

	list->next->prev = list; list->prev->next = list;
}

#endif // _LLIST_H_

