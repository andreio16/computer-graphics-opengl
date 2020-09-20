/* 
 * incarcare_textura.c
 * Rodica Baciu - Programarea aplicatiilor grafice 3D cu OpenGL
 *
 *  Programul mapeaza texturi diferite, incarcate din fisiere *.BMP,
 *  pe doua patrate.  
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void incarca_textura(const char* s);
void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLuint IDtextura;

void incarca_textura(const char* s)
{   
	AUX_RGBImageRec *pImagineTextura = auxDIBImageLoad(s);

    if(pImagineTextura != NULL )
	{
        glGenTextures( 1, &IDtextura );

		glBindTexture(GL_TEXTURE_2D, IDtextura);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}

	if(pImagineTextura)
	{
		if(pImagineTextura->data )
			free(pImagineTextura->data );

		free(pImagineTextura);
	}

}

//se fac setarile inclusiv cele pentru texturare
void myinit(void)
{    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

void CALLBACK display(void)
{ const char* sir;
	GLuint ID1, ID2;
    glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	sir=".\\covor.bmp";
	incarca_textura(sir);
	ID1=IDtextura;
	sir=".\\parchet.bmp";
	incarca_textura(sir);
	ID2=IDtextura;

	//primul patrat
	glBindTexture( GL_TEXTURE_2D, ID1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -1.0, 0.0);
		glTexCoord2f(0.0, 2.0); glVertex3f(-2.5, 1.0, 0.0);
		glTexCoord2f(2.0, 2.0); glVertex3f(-0.5, 1.0, 0.0);
		glTexCoord2f(2.0, 0.0); glVertex3f(-0.5, -1.0, 0.0);
	glEnd();

	//al doilea patrat
	glBindTexture( GL_TEXTURE_2D, ID2);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -1.0, 0.0);
		glTexCoord2f(0.0, 2.0); glVertex3f(0.5, 1.0, 0.0);
		glTexCoord2f(2.0, 2.0); glVertex3f(2.5, 1.0, 0);
		glTexCoord2f(2.0, 0.0); glVertex3f(2.5, -1.0, 0);
    glEnd();
   
	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Incarcarea texturilor din fisiere *.BMP");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}





