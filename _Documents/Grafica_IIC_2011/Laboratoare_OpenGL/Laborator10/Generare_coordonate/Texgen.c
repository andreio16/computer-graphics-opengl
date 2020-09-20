/*  texgen.c
 *  Acest program mapeazã o texturã pe un ceainic
 *  cu generarea automata a coordonatelor de texturare.  Textura
 *  este redatã ca hasuri pe ceainic.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void makeStripeImage(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

#define stripeImageWidth 32
GLubyte stripeImage[3*stripeImageWidth];

void makeStripeImage(void)
{ //construieste o textura 1D cu 3 octeti pentru fiecare texel
	//din 32 texeli 5 sunt rosii si 27 sunt verzi
    int j;
    
    for (j = 0; j < stripeImageWidth; j++) {
        stripeImage[3*j] = (j<=4) ? 255 : 0; //rosu
        stripeImage[3*j+1] = (j>4) ? 255 : 0; //verde
        stripeImage[3*j+2] = 0; //galben
    }
}

/* glTexGen stuff: */

GLfloat sgenparams[] = {1.0, 1.0, 1.0, 0.0};//coeficientii planului fata
// de care se calculeazã directia texturii

void myinit(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    makeStripeImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//incercati si parametrul GL_DECAL. Veti constata ca nu se tine seama 
	//de iluminare
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//incercati si cu GL_CLAMP. Nu se repeta texturarea. Se prelungec aceeiasi
	//texel-i
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, stripeImageWidth, 0,
         GL_RGB, GL_UNSIGNED_BYTE, stripeImage); //specificare texturii

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );//incercati si GL_OBJECT_PLANE
    glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);	//incercati si parametrul 
	//GL_EYE_PLANE
	//ascunderea suprafeþelor
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_GEN_S);//se genereaza automat coordonatele texturii pe s
    glEnable(GL_TEXTURE_1D);//texturare 1D
    //tãierea feþelor spate
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
    glCullFace(GL_BACK);
	//iluminarea
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	//calcularea automata a normalelor pentru coordonate generate cu GL_MAP1_VERTEX
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE); //normalizarea normalelor
    
    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);//exponentul pentru strãlucire
}

void CALLBACK display(void)
{
	int i=0;
	for(i=0; i<=360; i+=30)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix ();
		glRotatef(i, 1.0, 1.0, 1.0);
		auxSolidTeapot(2.0);
		glPopMatrix ();
		glFlush();
		Sleep(300);
	}
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    glOrtho (-3.5, 3.5, -3.5*(GLfloat)h/(GLfloat)w, 
        3.5*(GLfloat)h/(GLfloat)w, -3.5, 3.5);
    else
    glOrtho (-3.5*(GLfloat)w/(GLfloat)h, 
        3.5*(GLfloat)w/(GLfloat)h, -3.5, 3.5, -3.5, 3.5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 200, 200);
    auxInitWindow ("Texture Map");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
