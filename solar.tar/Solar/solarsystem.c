/* solarsystem.c - Jon McCormack, April 2004  */

/* Program to demonstrate the use of nested transformations.
 * This program draws a simple solar system model with a sun, a single
 * planet with an orbiting moon. It uses a virtual trackball to manipulate the solar system
 * and also shows how to do OpenGL lighting, z-buffering and smooth shading.
 *
 * See the supplied README for more details
 */


/* Copyright 2004 Jon McCormack
 * Last modified 24 April 2004 - Initial release
 */
#define SS_VERSION			1.0

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "trackball.h"
#include "material.h"
#include "text.h"

/* slices and stacks for the glut sphere */
#define SPHERE_SLICES			20
#define SPHERE_STACKS			20
#define RING_SLICES				40
#define SPHERE_SCALE_F			25.0		/* slice/stack reduction based on size */

/* solar system properties */
#define SUN_RADIUS				50
#define EARTH_TO_SUN_DISTANCE	100
#define EARTH_RADIUS			10
#define EARTH_TILT				6.0
#define MOON_TO_EARTH_DISTANCE	20
#define MOON_RADIUS				5.0
#define NEAR_CLIP				1.0
#define FAR_CLIP				2000.0

#define TINC_DEFAULT			0.2		/* default time increment in hours */
#define MAX_BUF					255		/* text storage buffer size */

void initSystem(void);
void setCamera(void);

/* user globals */
typedef struct {
	bool wireframe;		/* wireframe/shaded display */
	bool lighting;		/* light scene if true */
	bool smooth;		/* use smooth shading if true */
	bool zBuffering;	/* use z buffering if true */
	bool showOrbits;	/* show orbits if true */
	float camY;			/* distance of camera in y */
	float fovy;			/* camera field of view in y */
	double aspect;		/* camera aspect ratio */
	int	width;			/* viewport width in pixels */
	int height;			/* viewport height in pixels */
	double tInc;		/* time increment per display */
	double currentHour;	/* current time of day in hours */
	int currentDay;		/* current day of the year */
} UserOptions;

/* global user options init list */
UserOptions gUserOptions = { false, true, true, true, true,
							 300, 60.0, 1.0, 500, 500, TINC_DEFAULT, 0.0, 0 };

/*
 * userSettings
 *
 * Changes OpenGL options based on UserOptions settings
 */
void userSettings() {
	static GLfloat l0pos[4] = {0.0F, 0.0F, 0.0F, 1.0F};
	static GLfloat l1pos[4] = {500.0F, 500.0F, 500.0F, 0.0F};
	static GLfloat ldir[3] = {1.0F, 0.0F, 0.0F };

	initSystem(); /* reset transforms */
	
	setLight(GL_LIGHT0, &whiteLight);
	setLightPosition(GL_LIGHT0, l0pos, ldir);
	setLight(GL_LIGHT1, &blueLight);
	setLightPosition(GL_LIGHT1, l1pos, ldir);

	setMaterial(&greyMaterial);

	if (gUserOptions.lighting) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	 	glEnable(GL_LIGHT1);
	} else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}

	if (gUserOptions.smooth) {
		glShadeModel(GL_SMOOTH);
	} else {
		glShadeModel(GL_FLAT);
	}

	if (gUserOptions.zBuffering) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	setCamera();
}

/*
 * drawSphere
 *
 * draws a sphere of supplied radius using glut functions.
 * uses wireframe option in gUserOptions. Having display lists would be faster if repeated drawing is
 * going on...
 *
 */
void drawSphere(GLdouble radius) {
	if (gUserOptions.wireframe)
		glutWireSphere(radius, SPHERE_SLICES * radius/SPHERE_SCALE_F, SPHERE_STACKS * radius/SPHERE_SCALE_F);
	else
		glutSolidSphere(radius, SPHERE_SLICES, SPHERE_STACKS);
}

/*
 * drawCircle
 *
 * draws a circle of given radius in the y = 0 plane
 */
void drawCircle(GLdouble radius) {
	register int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < RING_SLICES; ++i) {
		glVertex3d(radius * sin(2 * M_PI/(double)RING_SLICES * (double)i),
				   0.0,
				   radius * cos(2 * M_PI/(double)RING_SLICES * (double)i));
	}
	glEnd();
}

/*
 * drawSolarSystem
 *
 * draws the simple solar system:
 *      - sun at the origin
 * 	    - earth that rotates about its own polar axis once per day and also orbits the sun
 *      - earth has a tilt off its axis from +y
 *      - moon orbits the earth once every 28 days
 */
void drawSolarSystem(int dayOfTheYear, double timeOfDay) {
	/* draw sun  at world coordinate origin  with poles in the y axis */
	glPushMatrix();
	 glRotated(90.0, 1.0, 0.0, 0.0);
	 setMaterial(&yellowMaterial);
	 glColor3fv(yellowColour);
	 drawSphere(SUN_RADIUS);
	glPopMatrix();

	/* draw Earth */
	/* Earth rotates about its axis once every 24 hour period and orbits
	 * the sun once every 365 days
	 */
	setMaterial(&greyMaterial);
	glColor3fv(greyColour);
	glPushMatrix();
	 if (gUserOptions.showOrbits) drawCircle(EARTH_TO_SUN_DISTANCE);
	 glRotated(((double)dayOfTheYear + timeOfDay/24.0)/365.0 * 360.0, 0.0, 1.0, 0.0);
	 glTranslated(0.0, 0.0, EARTH_TO_SUN_DISTANCE);
	 glPushMatrix();
	  glRotated(EARTH_TILT, 0.0, 0.0, 1.0);
	  if (gUserOptions.showOrbits) drawCircle(MOON_TO_EARTH_DISTANCE);
	  glRotated(timeOfDay / 24.0 * 360.0, 0.0, 1.0, 0.0);
	  glRotated(90.0, 1.0, 0.0, 0.0);
	  drawSphere(EARTH_RADIUS);
	 glPopMatrix();
	
	 /* draw moon */
	 /* Moon orbits the Earth once every 28 days */
	 glPushMatrix();
	  glRotated(EARTH_TILT, 0.0, 0.0, 1.0);
	  glRotated(((double)(dayOfTheYear % 28)+ timeOfDay/24.0)/28.0 * 360.0, 0.0, 1.0, 0.0);
	  glTranslated(0.0, 0.0, MOON_TO_EARTH_DISTANCE);
	  setMaterial(&redPlasticMaterial);
	  glColor3fv(redColour);
	  glRotated(90.0, 1.0, 0.0, 0.0);
	  drawSphere(MOON_RADIUS);
	 glPopMatrix();

	glPopMatrix();
}

/*
 * display
 *
 * This function is called by the GLUT to display the graphics
 *
 */
void display(void)
{
	static char textBuffer[MAX_BUF];

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* set matrix mode to modelview */
    glMatrixMode(GL_MODELVIEW);

	/* apply the trackball transform */
	applyTrackball();

	/* save matrix */
	glPushMatrix();


	/* draw the geometry */
	drawSolarSystem(gUserOptions.currentDay, gUserOptions.currentHour);

	/* display current text */
	sprintf(textBuffer,"solarsystem v%2.1f: stardate %5d days, %5.2f hours", SS_VERSION,
								gUserOptions.currentDay, gUserOptions.currentHour);
	display2DText(5,10, textBuffer, gUserOptions.width, gUserOptions.height);

	/* restore matrix */
	glPopMatrix();

	/* swap buffers to display the frame */
 	glutSwapBuffers();
}

/*
 * setCamera
 *
 * sets the camera to be looking from the +ive y axis looking toward the origin
 */
void setCamera() {
	/* 
	 * set viewing window using perspective projection
	 */
    glMatrixMode(GL_PROJECTION); 

	/* perspective parameters: field of view, aspect, near clip, far clip */
	gluPerspective( gUserOptions.fovy, gUserOptions.aspect, NEAR_CLIP, FAR_CLIP);

	/* set matrix mode to modelview */
    glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, gUserOptions.camY, 0.0, /* eye position */
			  0.0, 0.0, 0.0, /* lookat point */
			  0.0, 0.0, 1.0); /* up is in +ive y */
}
	

/*
 * myReshape
 *
 * This function is called whenever the user (or OS) reshapes the
 * OpenGL window. The GLUT sends the new window dimensions (x,y)
 *
 */
void myReshape(int w, int h)
{
	/* set viewport to new width and height */
	/* note that this command does not change the CTM */
    glViewport(0, 0, w, h);
	initSystem();

	/* set trackball window size */
	trackballResize(w, h);
	gUserOptions.aspect = (GLdouble)w/(GLdouble)h;
	gUserOptions.width = w;
	gUserOptions.height = h;
	setCamera();
}

/*
 * myKey
 *
 * responds to key presses from the user
 */
void myKey(unsigned char k, int x, int y)
{
	switch (k) {
	case 'q':
	case 'Q':	exit(0);
		break;

	case ',':
			gUserOptions.camY += 100;
			initSystem();
			setCamera();
			break;
	case '.':
			gUserOptions.camY -= 100;
			initSystem();
			setCamera();
			break;
	case 'z':
			gUserOptions.fovy += 10;
			initSystem();
			setCamera();
			break;
	case 'Z':
			gUserOptions.fovy -= 10;
			initSystem();
			setCamera();
			break;
	case '+':
			gUserOptions.tInc *= 2.0;
			break;
	case '-':
			gUserOptions.tInc *= 0.5;
			break;
	case 't':
			gUserOptions.tInc = TINC_DEFAULT;
			break;
	default:
		printf("Unknown keyboard command \'%c\'.\n", k);
		break;
	}
}


/*
 * myMouse
 *
 * function called by the GLUT when the user presses a mouse button
 *
 * Here we increment the global rotation rate with each press - left to do a
 * positive increment, right for negative, middle to reset
 */
void myMouse(int btn, int state, int x, int y)
{   
	
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) gUserOptions.tInc *= 2.0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) gUserOptions.tInc = 0.5;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) gUserOptions.tInc *= 0.5;

	/* force redisplay */
	glutPostRedisplay();
}   

/*
 * myIdleFunc
 *
 * increments the rotation variable within glutMainLoop
 */
void myIdleFunc(void) {

	gUserOptions.currentHour += gUserOptions.tInc;
	if (gUserOptions.currentHour >= 24.0) {
		gUserOptions.currentHour = 0.0;
		gUserOptions.currentDay += 1;
		gUserOptions.currentDay %= 365;
	}

	/* force glut to call the display function */
	glutPostRedisplay();
}

/*
 * userMenu 
 *  
 * callback, called by the GLUT when the user selects a menu option.
 * This function currently contains "magic numbers" in that the menu numbers
 * and their functions are hard coded. A better option would be to use macros
 * or have a data structure that relates menu number, name and function...
 */ 
void userMenu(int value) {
    switch(value) {
    case 1:
		gUserOptions.wireframe = !gUserOptions.wireframe;
		userSettings();	
        break;
    case 2:
		gUserOptions.lighting = !gUserOptions.lighting;
		userSettings();	
        break;
    case 3:
		gUserOptions.smooth = !gUserOptions.smooth;
		userSettings();	
        break;
    case 4:
		gUserOptions.zBuffering = !gUserOptions.zBuffering;
		userSettings();	
        break;
    case 5:
		gUserOptions.showOrbits = !gUserOptions.showOrbits;
		userSettings();	
        break;
    default:
        fprintf(stderr, "solarsystem: unknown menu option %d\n", value);
        break;
    }
}

	

void initSystem() {
	
	/* set clear colour */
	glClearColor(0.0, 0.0, 0.0, 1.0);

	/* set current colour to white */
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * main
 *
 * Initialization and sets graphics callbacks
 *
 */
int main(int argc, char **argv)
{
	/* glutInit MUST be called before any other GLUT/OpenGL calls */
    glutInit(&argc, argv);

	/* set double buffering, z buffering */
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Solar System");

	/* set callback functions */
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutIdleFunc(myIdleFunc);
	glutKeyboardFunc(myKey);
	glutMouseFunc(trackballButton);
	glutMotionFunc(trackballMotion);

	 /* set up right menu */
    glutCreateMenu(userMenu);
    glutAddMenuEntry("wireframe", 1);
    glutAddMenuEntry("lighting", 2);
    glutAddMenuEntry("smooth shading", 3);
    glutAddMenuEntry("z-buffering", 4);
    glutAddMenuEntry("orbit rings", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


/*	initSystem(); */
	userSettings();	
    glutMainLoop();
	 
	return 0;
}
