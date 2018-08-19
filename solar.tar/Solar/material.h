/* material.h - Jon McCormack, April 2004  */

/*
 * data structures and utility functions for dealing with OpenGL materials
 * and lights
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <gl.h>
#endif

/* structure for a material */
typedef struct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} Material;

/* structure for a light */
typedef struct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} Light;

/* define some basic materials, colours and lights */

extern Material redPlasticMaterial; 
extern Material greyMaterial;
extern Material yellowMaterial;
extern float redColour[3];
extern float greyColour[3];
extern float yellowColour[3];
extern Light whiteLight;
extern Light blueLight;

/* forward declare functions in material.c */
void setMaterial(Material * matP);
void setLight(GLenum, Light * lightP);
void setLightPosition(GLenum, GLfloat pos[4], GLfloat spotDir[3]);

#endif
