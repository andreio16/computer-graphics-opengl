/* 
 * amestec.c
 * Rodica Baciu - Programarea aplicatiilor grafice 3D cu OpenGL
 *
 *  Programul deseneaza niste dreptunghiuri care se suprapun
 *  pentru a demonstra efectul pe care il are ordinea desenarii 
 *  in amestecarea culorilor in functie de alfa.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

/*  Se initializeaza functia de amestecare.
 */
void myinit(void)
{
    glEnable (GL_BLEND); //activarea amestecarii
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //factorii sursa si destinatie
    glShadeModel (GL_FLAT);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void CALLBACK display(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0); //culoarea fundalului
	glClear(GL_COLOR_BUFFER_BIT);
/*  deseneaza doua dreptunghiuri care se suprapun ca un patrat stanga jos  */
    glColor4f (1.0, 0.0, 0.0, 0.75); //dreptunghiul rosu
    glRectf (0.0, 0.0, 0.5, 1.0);
	glFlush();
	
	Sleep(2000);	//dreptunghiul verde
    glColor4f (0.0, 1.0, 0.0, 0.75);
    glRectf (0.0, 0.0, 1.0, 0.5);
	glFlush();

/*  deseneaza doua patrate care se suprapun dreapta sus */
    
	Sleep(2000);  //patratul verde
	glColor4f (0.0, 1.0, 0.0, 0.75);
    glRectf (0.5, 0.5, 1.0, 1.0);
	glFlush();

	Sleep(2000);   //patratul rosu
    glColor4f (1.0, 0.0, 0.0, 0.75);
    glRectf (0.5, 0.5, 1.0, 1.0);

    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if(!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
	// coltul stanga jos la (0, 0) si dreapta sus la (1, 1)
    gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
    else 
    gluOrtho2D (0.0, 1.0*(GLfloat)w/(GLfloat)h, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Amestecarea culorilor in functie de alfa");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
