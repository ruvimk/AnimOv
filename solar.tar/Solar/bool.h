/* bool.c - Jon McCormack, April 2004  */

/*
 * add boolean type to C
 */

#ifndef _BOOL_H_
#define _BOOL_H_

#if defined(true) | defined(false)
#error Boolean types seem to have been defined somewhere else
#else

typedef unsigned short bool;
#define false		0
#define true		1

#endif

#endif
