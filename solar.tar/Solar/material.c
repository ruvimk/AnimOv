/* material.c - Jon McCormack, April 2004  */

/*
 * utility library for using basic materials and lights in OpenGL
 */

/* Copyright 2004 Jon McCormack
 * Last modified 24 April 2004
 */

#include "material.h"

/* define some basic materials, colours and lights as globals */

Material redPlasticMaterial = {
    {0.2F, 0.0F, 0.0F, 1.0F},
    {0.8F, 0.0F, 0.0F, 1.0F},
    {0.8F, 0.6F, 0.6F, 1.0F},
     32.0F
};

Material greyMaterial = {
    {0.2F, 0.2F, 0.2F, 1.0F},
    {0.8F, 0.8F, 0.8F, 1.0F},
    {0.8F, 0.8F, 0.8F, 1.0F},
     50.0F
};

Material yellowMaterial = {
    {0.2F, 0.18F, 0.0F, 1.0F},
    {0.8F, 0.7F, 0.0F, 1.0F},
    {0.8F, 0.7F, 0.0F, 1.0F},
     50.0F
};

/* basic colours to match materials */
float redColour[3] = {0.8F, 0.0F, 0.0F};
float greyColour[3] = {0.8F, 0.8F, 0.8F};
float yellowColour[3] = {0.8F, 0.7F, 0.0F};

Light whiteLight = {
    {0.0F, 0.0F, 0.0F, 1.0F},
    {1.0F, 1.0F, 1.0F, 1.0F},
    {1.0F, 1.0F, 1.0F, 1.0F}
};

Light blueLight = {
    {0.0F, 0.0F, 0.1F, 1.0F},
    {0.0F, 0.0F, 1.0F, 1.0F},
    {0.3F, 0.2F, 1.0F, 1.0F}
};

/*********************************************/
/*
 * setMaterial
 *
 * sets the current material
 */ 
void setMaterial( Material * matP)
{
    /* material properties for front faces only! */
	glMaterialfv(GL_FRONT, GL_AMBIENT, matP->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matP->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matP->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, matP->shininess);
}

/*
 * setLight
 *
 * set the OpenGL light to the supplied Light * 
 */
void setLight( GLenum lightNumber, Light * lightP ) {

    glLightfv(lightNumber, GL_AMBIENT, lightP->ambient);
    glLightfv(lightNumber, GL_DIFFUSE, lightP->diffuse);
    glLightfv(lightNumber, GL_SPECULAR, lightP->specular);

    glLightf(lightNumber, GL_SPOT_EXPONENT, 0.0F);
    glLightf(lightNumber, GL_SPOT_CUTOFF, 180.0F);
    glLightf(lightNumber, GL_CONSTANT_ATTENUATION, 1.0F);
    glLightf(lightNumber, GL_LINEAR_ATTENUATION, 0.0F);
    glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, 0.0F);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
}

/*
 * setLightPosition
 *
 * sets the light to supplied position and direction (for spot lights)
 */
void setLightPosition(GLenum lightNumber, GLfloat pos[4], GLfloat spotDir[3]) {

    glLightfv(lightNumber, GL_POSITION, pos);
    glLightfv(lightNumber, GL_SPOT_DIRECTION, spotDir);
}

