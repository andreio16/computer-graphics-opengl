/*  Wire_Bezier.c
 *  Programul realizeaza o reprezentare Wireframe
 *  pentru o suprafata Bezier, utilizand evaluatori bidimensionali
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
// se definesc cele 16 puncte de control
GLfloat ctrlpoints[4][4][3] = {
    {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
	{0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
    {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
	{0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
    {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
	{0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
    {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
	{0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

void CALLBACK display(void)
{
    int i, j;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// se foloseste si buffer de rfresh si buffer de adancime
    glColor3f(1.0, 0.0, 0.0); // culoarea curenta
    glPushMatrix (); // pentru a nu aplica transformari geometrice
	// la fiecare redesenare a ferestrei
    glRotatef(85.0, 1.0, 1.0, 1.0); // rotatie in jurul axei (1, 1, 1)
    // urmeaza desenarea wireframe a suprafetei
	//fiecare patch (8X8 patch-uri) este desenat dintr-o linie franta
	//din 30 de segmente de dreapta
	for (j = 0; j <= 8; j++) { 
	glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++)
	    glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0); // evalueaza un punct
		// pe suprafata pentru valorile u si v ale ale parametrilor
	glEnd();
	glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++)
	    glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
	glEnd();
    }
    glPopMatrix ();
    glFlush();
}

void myinit(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0); // culoarea background-ului

/*	 functia glMap2f defineste caracteristicile suprafetei Bezier
	- tipul punctelor determinate de functia glEvalCoord2f
	- intervalul de variatie al parametrului u (0 -1 in acest caz)
	- intervalul valorilor in tabloul ctrlpoints intre doua puncte
	de control pe directia u
	- numarul punctelor de control pe directia u
	- intervalul de variatie al parametrului v (0 -1 in acest caz)
	- intervalul valorilor in tabloul ctrlpoints intre doua puncte
	de control pe directia v
	- numarul punctelor de control pe directia v
	-tabloul punctelor de control
*/
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
	    0, 1, 12, 4, &ctrlpoints[0][0][0]);
	
    glEnable(GL_MAP2_VERTEX_3);  // validarea tipului de evaluare GL_MAP2_VERTEX_3
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0); // intervalele de esantionare 
	// a suprafetei pentru parametrii u si v
	glEnable(GL_DEPTH_TEST);  // se activeaza ascunderea suprafetelor
    glShadeModel(GL_FLAT);  // umbrire pologonala constanta
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
	glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w, 
	    4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
    else
	glOrtho(-4.0*(GLfloat)w/(GLfloat)h, 
	    4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow (" Suprafata Bezier wireframe");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
