/* text.h - Jon McCormack, April 2004  */

/*
 * Utility functions dealing with text drawing
 */

#ifndef _TEXT_H_
#define _TEXT_H_


/* forward declare functions in text.c */
void writeText(int x, int y, const char * mesg);
void display2DText(int x, int y, const char * mesg, int width, int height);

#endif
