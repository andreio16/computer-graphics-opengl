/*
Programul afiseaza un cub care se roteste
*/
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK IdleFunction(void);

void myinit (void) {
    glClearColor(1.0, 1.0,1.0, 1.0);
	glColor3f(0.0,0.0,0.0);
}

void CALLBACK display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    auxWireCube(100);
	glFlush();
}

void CALLBACK IdleFunction(void)
{
	glRotatef(30,1,1,1);
	display();
	Sleep(300);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
    glOrtho (-160.0, 160.0, -160.0*(GLfloat)h/(GLfloat)w, 
        160.0*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else 
    glOrtho (-160.0*(GLfloat)w/(GLfloat)h, 
        160.0*(GLfloat)w/(GLfloat)h, -160.0, 160.0, -80.0, 80.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 300, 200);
    auxInitWindow ("Un cub care se roteste");
    myinit ();
	auxReshapeFunc (myReshape);
	auxIdleFunc(IdleFunction);
    auxMainLoop(display);
    return(0);
}
