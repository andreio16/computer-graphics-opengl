/*
 *  decupare.c
 *  Decuparea obiectelor dupa plane de decupare
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
void initlights(void);

void initlights(void)
{
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
void CALLBACK display(void)
{	// se definesc coeficientii A, B, C, D din ecuatia planului
	// de decupare GL_CLIP_PLANE0, |ecuatia: x=1
    GLdouble eqn0[4] = {1.0, 0.0, 0.0, 1.0}; 
	// se definesc coeficientii A, B, C, D din ecuatia planului
	// de decupare GL_CLIP_PLANE1, |ecuatia: -x=1
    GLdouble eqn1[4] = {-1.0, 0.0, 0.0, 1.0};
	// se definesc coeficientii A, B, C, D din ecuatia planului
	// de decupare GL_CLIP_PLANE2; |ecuatia: y=1
	GLdouble eqn2[4] = {0.0, 1.0, 0.0, 1.0};
	// se definesc coeficientii A, B, C, D din ecuatia planului
	// de decupare GL_CLIP_PLANE3|ecuatia: -y=1
	GLdouble eqn3[4] = {0.0, -1.0, 0.0, 1.0};


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f (1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef (0.0, 0.0, -5.0);

/*    se defineste planul de decupare  GL_CLIP_PLANE0    */
    glClipPlane (GL_CLIP_PLANE0, eqn0);
/*    se activeaza planul de decupare  GL_CLIP_PLANE0    */
    glEnable (GL_CLIP_PLANE0);

	/*   planul de decupare  GL_CLIP_PLANE1	*/
    glClipPlane (GL_CLIP_PLANE1, eqn1);
    glEnable (GL_CLIP_PLANE1);

		/*   planul de decupare  GL_CLIP_PLANE2	*/
	glClipPlane (GL_CLIP_PLANE2, eqn2);
    glEnable (GL_CLIP_PLANE2);

		/*   planul de decupare  GL_CLIP_PLANE3	*/

	 glClipPlane (GL_CLIP_PLANE3, eqn3);
    glEnable (GL_CLIP_PLANE3);

    glRotatef (90.0, 1.0, 1.0, 0.0);
	auxSolidSphere(2.0);
    glPopMatrix();
    glFlush();
}

void myinit (void) {
    //glShadeModel (GL_FLAT);
	initlights();
	glEnable(GL_DEPTH_TEST);

}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Functia principala
 *  Deschide fereastra cu dimensiunile fixate, cu bara de titlu  
 *  modelul culorilor RGB, trateaza evenimentele de intrare.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Plane de decupare");
    myinit ();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
