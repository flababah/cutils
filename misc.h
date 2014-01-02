/*
 *    Copyright (c) 2013 Anders Høst
 *
 *    Miscellaneous macros.
 *
 *    > likely, unlikely     Optimize code path for specific branch.
 *    > MIN, MAX             Safe min/max macros.
 *    > ARR_SIZE             Get length of array at compile-time.
 */

#ifndef _MISC_H_
#define _MISC_H_

#define   likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define __CMP(binop, a, b) ({ \
	__typeof__(a) __a = (a); \
	__typeof__(b) __b = (b); \
	__a binop __b ? __a : __b; \
})

#define MAX(a, b) __CMP(>, a, b)
#define MIN(a, b) __CMP(<, a, b)

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif // _MISC_H_

