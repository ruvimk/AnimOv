/* trackball.c - Jon McCormack, April 2004  */

/*
 * utility library to create a virtual trackball for object/camera manipulation
 */

/* Copyright 2004 Jon McCormack
 * Last modified 27 April 2004
 */

#include "trackball.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif
#include <math.h>

/* init global trackball data structure */
static TrackballData gTrackballData = { 0.0, 
								{0.0,0.0,0.0},
								false, false, false,
								{0.0,0.0,0.0},
								0, 0};

void trackball_ptov(int x, int y, float v[3]) {
    float d, a;
	int width = gTrackballData.windowWidth;
	int height = gTrackballData.windowHeight;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

/*
 * trackballMotion
 *
 * callback usually tied to glut callback glutMotionFunc
 * allows the user to move an object around using the mouse
 */
void trackballMotion(int x, int y) {

    float curPos[3], dx, dy, dz;

    trackball_ptov(x, y, curPos);

	if(gTrackballData.tracking) {
    	dx = curPos[0] - gTrackballData.lastPos[0];
    	dy = curPos[1] - gTrackballData.lastPos[1];
    	dz = curPos[2] - gTrackballData.lastPos[2];

    	if (dx || dy || dz) {
    		gTrackballData.angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);

    		gTrackballData.axis[0] = gTrackballData.lastPos[1]*curPos[2] - 
										gTrackballData.lastPos[2]*curPos[1];
    		gTrackballData.axis[1] = gTrackballData.lastPos[2]*curPos[0] - 
										gTrackballData.lastPos[0]*curPos[2];
    		gTrackballData.axis[2] = gTrackballData.lastPos[0]*curPos[1] - 
										gTrackballData.lastPos[1]*curPos[0];

    		gTrackballData.lastPos[0] = curPos[0];
    		gTrackballData.lastPos[1] = curPos[1];
    		gTrackballData.lastPos[2] = curPos[2];
			gTrackballData.updated = false;
    	}
	}
    glutPostRedisplay();
}

void startTrackballMotion(int x, int y) {
	gTrackballData.tracking = true;
	gTrackballData.moving = true;
	trackball_ptov(x, y, gTrackballData.lastPos);
}

void stopTrackballMotion(int x, int y) {
	gTrackballData.angle = 0.0;
	gTrackballData.tracking = false;
	gTrackballData.moving = false;
	gTrackballData.updated = false;
}


/*
 * trackballButton
 *
 * callback to be used when the user presses a mouse button
 * the value of TRACKBALL_BUTTON is defined in trackball.h
 */
void trackballButton(int button, int state, int x, int y) {
	if (button == TRACKBALL_BUTTON)
		switch(state) {
			case GLUT_DOWN:
				startTrackballMotion(x, gTrackballData.windowHeight - y);
				break;
			case GLUT_UP:
				stopTrackballMotion(x,y);
				break;
		}
}

/* 
 * trackballResize
 *
 * must be called when the graphics window area is changed
 * Also can be used to init part of gTrackballData
 *
 */
void trackballResize(int width, int height) {
	gTrackballData.windowWidth = width;
	gTrackballData.windowHeight = height;
}

/*
 * applyTrackball
 *
 * apply the current trackball rotation as a glRotate call
 * This function is usually called within the display function
 */
void applyTrackball() {
/*	printf("mov: %s, track: %s, axis: <%5.3f,%5.3f,%5.3f>, angle: %6.2f\n",
		gTrackballData.moving ? "on" : "off",
		gTrackballData.tracking ? "on" : "off",
		gTrackballData.axis[0], gTrackballData.axis[1], gTrackballData.axis[2],
		gTrackballData.angle); */
	if (gTrackballData.moving && !gTrackballData.updated) {
		glRotatef(-gTrackballData.angle, gTrackballData.axis[0],
				  gTrackballData.axis[1], gTrackballData.axis[2]);
		gTrackballData.updated = true;
	}
}
