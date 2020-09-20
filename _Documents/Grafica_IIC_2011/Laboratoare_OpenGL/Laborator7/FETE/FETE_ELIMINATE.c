/*
 Eliminarea fetelor functie de orientare
 Se va afisa fara activarea eliminarii fetelor. Cu orientarea 
 directa implicita se  vor schimba fetele care se elimina-
 cele fata sau cele spate-cu functia glCullFace.
 Se va schimba apoi ordinea directa a varfurilor cu glFrontFace
 avand parametrii - GL_CW, GL_CCW.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);


void myinit(void)
{
  
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
}


void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 0.0, 0.0);
	//glEnable(GL_CULL_FACE);//activeaza eliminarea fetelor
	//glCullFace(GL_BACK);//sunt eliminate fetele spate
	//inlocuiti cu GL_FRONT pentru fete fata

    glBegin(GL_POLYGON);//orientare GL_CCW-implicita ca orientare directa
		glVertex2f(-1.0, 0.0);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
	glEnd();

	glColor3f (0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);//orientare GL_CW-implicita ca orientare indirecta
		glVertex2f(1.0, 1.0);
		glVertex2f(2.0, 1.0);
		glVertex2f(2.0, 0.0);
		glVertex2f(1.0, 0.0);
	glEnd();

    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, (GLfloat) w/(GLfloat) h, 3.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    glTranslatef (0.0, 0.0, -4.0);  
}

/*  Main Loop
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB );
    auxInitPosition (0, 0, 500, 400);
    auxInitWindow ("Eliminarea fetelor functie de orientare");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
