/*
 *  robot.c
 *  Compunerea transformarilor geometrice pentru desenarea obiectelor 
 *  animate ierarhice.
 *  Intercatiune:  sagetile 
 *  
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void drawPlane(void);
void CALLBACK elbowAdd (void);
void CALLBACK elbowSubtract (void);
void CALLBACK shoulderAdd (void);
void CALLBACK shoulderSubtract (void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

static int shoulder = 0, elbow = 0;

void CALLBACK elbowAdd (void)
{
    elbow = (elbow + 5) % 360;
}

void CALLBACK elbowSubtract (void)
{
    elbow = (elbow - 5) % 360;
}

void CALLBACK shoulderAdd (void)
{
    shoulder = (shoulder + 5) % 360;
}

void CALLBACK shoulderSubtract (void)
{
    shoulder = (shoulder - 5) % 360;
}

void CALLBACK display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef (-1.0, 0.0, 0.0);
    glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
    glTranslatef (1.0, 0.0, 0.0);
    auxWireBox(2.0, 0.4, 1.0);

    glTranslatef (1.0, 0.0, 0.0);
    glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
    glTranslatef (1.0, 0.0, 0.0);
    auxWireBox(2.0, 0.4, 1.0);

    glPopMatrix();
    glFlush();
}

void myinit (void) 
{
    glShadeModel (GL_FLAT);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);  
}


int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 400, 400);
    auxInitWindow ("Composite Modeling Transformations");

    myinit ();

    auxKeyFunc (AUX_LEFT, shoulderSubtract);
    auxKeyFunc (AUX_RIGHT, shoulderAdd);
    auxKeyFunc (AUX_UP, elbowAdd);
    auxKeyFunc (AUX_DOWN, elbowSubtract);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
