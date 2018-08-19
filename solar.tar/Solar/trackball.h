/* trackball.h - Jon McCormack, April 2004  */

/* Code to simulate a virtual "trackball" - a manipulator that allows
 * interactive rotation of an object as though it were surrounded by an
 * imaginary ball.
 * Based on Hultquist, Jeff: "A Virtual Trackball", in Andrew Glassner (ed).
 * Graphics Gems, Academic Press, 1990, pp. 462-463
 * and code from E. Angel: "Interactive Computer Graphics: A Top-Down Approach
 * with OpenGL", Third Ed., Addison-Wesley Longman, 2003.
 *
 */

#ifndef _TRACKBALL_H_
#define _TRACKBALL_H_

#include "bool.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

/* mouse button that will be used to control the trackball - can be changed */
#define TRACKBALL_BUTTON	GLUT_LEFT_BUTTON

/* data structure for trackball */
typedef struct {
	float		angle;		/* rotation angle */
	float		axis[3];	/* rotation axis */
	bool		tracking;	/* == true if we are tracking mouse */
	bool		moving;		/* == true if object should be moving */
	bool		updated;	/* == true if current rotation has been applied */
	float		lastPos[3];	/* previous trackball position */
	int			windowWidth;
	int			windowHeight;
} TrackballData;

/* forward declare global trackball data */
// extern TrackballData gTrackballData;

/* forward declare functions in trackball.c */
void trackballMotion(int x, int y); /* glut callback for mouse motion */
void trackballButton(int button, int state, int x, int y);
void trackballResize(int width, int height);
void applyTrackball();

#endif
