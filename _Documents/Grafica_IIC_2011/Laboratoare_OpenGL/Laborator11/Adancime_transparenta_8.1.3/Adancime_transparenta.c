/* 
 * Adancime_transparenta.c
 * Rodica Baciu - Programarea aplicatiilor grafice 3D cu OpenGL
 *
 *  Programul arata cum trebuie procedat in cazul in care ascunderea
 *  suprafetelor este rezolvata de algoritmul z-buffer si nu
 *  prin gestionarea ordinii reprezentarii obiectelor dar
 *  obiectele transparente nu ascund totusi total obiectele din spatele lor
 *  deci amestecarea este prezentã.
 */
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK comuta_pozitia_vizualiz(AUX_EVENTREC *event);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

void myinit(void)
{
//se fac setarile pentru iluminare 
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 0.15 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.15 };
    GLfloat mat_shininess[] = { 15.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
//se fac setarile pentru eliminarea suprafetelor ascunse
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

GLboolean poz_viz = GL_TRUE; //la momentul initial se priveste din fata
//la apasarea butonului stang al mouse-ului se comuta 
//pozitia de vizualizare
void CALLBACK comuta_pozitia_vizualiz(AUX_EVENTREC *event)
{
    if (poz_viz)
    poz_viz = GL_FALSE;
    else
    poz_viz = GL_TRUE;
}

void CALLBACK display(void)
{
    GLfloat position[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_sphere_opac[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_cube_transp[] = { 1.0, 1.0, 0.0, 0.25 };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    glPushMatrix ();
    
	if (poz_viz)
		//priveste din fata
        gluLookAt (0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
    else 
		//priveste din spate
        gluLookAt (0.0, 0.0, -9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
    //se deseneazã sfera fiind activatã ascunderea si dezactivatã amestecare
	glPushMatrix ();
    glTranslatef (0.0, 0.0, -1.3);
    //sfera a fost adusa mai in spate
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_sphere_opac);
    auxSolidSphere (1.0);
    glPopMatrix ();

   //se deseneazã cubul cu ascunderea si amestecarea activatã
	//dar cu buffer-ul z pe read-only

	glEnable (GL_BLEND);
    glDepthMask (GL_FALSE);  //buffer-ul z pe read-only
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cube_transp);
    glTranslatef (0.0, 0.0, 1.3); 
	//iar cubul a fost dus mai in fata
	glRotatef(30.0, 1.0, 1.0, 1.0);
    auxSolidCube (2.0);
    glDepthMask (GL_TRUE);
    glDisable (GL_BLEND);
    
	glPopMatrix ();
    glFlush ();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if(!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 250, 250);
    auxInitWindow ("Transparenta si ascunderea suprafetelor");
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, comuta_pozitia_vizualiz);
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
