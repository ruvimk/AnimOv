/* text.c - Jon McCormack March 2003  */

#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

/*
 * drawText
 *
 * Draws the specified message in both
 * raster and stroke text
 *
 */
void drawText(int x, int y, const char * message)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glRasterPos2i((GLfloat)x, (GLfloat)y);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message++);
	}
}

/*
 * display2DText
 *
 * displays 2D text by defining a new projection and modelview transform to match that of
 * the supplied viewport dimensions. NOTE: some older OpenGL implementations don't allow you
 * to push the projection matrix - in this case this function will fail.
 * We also save lighting and depth buffer attributes and set things up so lighting, depth buffer, colour
 * won't influence our text drawing.
 * The functions takes a 2D screen (viewport) coordinate for the start of the text, the text and the viewport
 * width and height.
 */
void display2DText(int x, int y, const char * mesg, int width, int height) {
	
	/* save lighting, depth buffer and colour attributes - use Client for speed */
	glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity(); 	/* clear projection matrix */
	gluOrtho2D(0,(GLdouble)width,0,(GLdouble)height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();	/* clear modelview matrix */

	glColor3f(0.3, 0.0, 1.0); /* text colour */
	drawText(x,y,mesg);
	glPopMatrix();		/* restore modelview matrix */

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();		/* restore projection matrix */
	
	glPopAttrib();		/* restore attributes */

	glMatrixMode(GL_MODELVIEW);	/* for safty! */
}
