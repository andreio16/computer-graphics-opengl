/*
 *  movelight.c
 *  Programul demonstreaza modul de modificare al pozitiei sursei de lumina
 *  utilizand functiile de transformare geometrica (translatie sau rotatie) 
 *  Pozitia sursei de lumina este modificatã dupa apelarea transformarii 
 *  Pozitia de vizualizare nu este modificata.
 *  
 *
 *  Se deseneaza un tor cu material de culoare gri
 *  Obiectul este iluminat de o singura sursa de lumina
 *
 *  Interactiune:  prin apasarea butonului stang al mouse-ului
 *  transformarea de modelare se modifica prin inmultirea cu matricea
 *  de rotatie cu 30 grade
 *  Scena este redesenata cu torul in noua pozitie.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK movelight (AUX_EVENTREC *event);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

static int spin = 0;

void CALLBACK movelight (AUX_EVENTREC *event)
{
    spin = (spin + 30) % 360;
}

void myinit (void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

/*  Pozitia sursei de lumina este resetata dupa ce se apleleaza
 *  transformarea de rotatie.  Aceasta plaseaza 
 *  sursa intr-o noua pozitie in coordonate reale.  Cubul
 *  reprezinta pozitia sursei de lumina.
 */
void CALLBACK display(void)
{
    GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 }; // pozitia susrei

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix ();
    glTranslatef (0.0, 0.0, -5.0); // pozitia de observare

    glPushMatrix ();
    glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);//rotatia sursei
    glRotated (0.0, 1.0, 0.0, 0.0);
    glLightfv (GL_LIGHT0, GL_POSITION, position); 	// functie apelata
	// doar dupa ce s-a stabilit matricea de rotaie

	// se deseneaza cubul corespunzând pozitiei sursei
    glTranslated (0.0, 0.0, 1.5);
    glDisable (GL_LIGHTING);
    glColor3f (0.0, 1.0, 1.0);
    auxWireCube (0.1);
    glEnable (GL_LIGHTING);
    glPopMatrix ();

    // se deseneaza torul
	auxSolidTorus (0.275, 0.85);
    glPopMatrix ();
    glFlush ();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Sursa îsi modificã pozitia la apãsarea butonului stâng al mouse-ului");
    myinit();
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, movelight);// inregistreaza functia callback moveligh
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
