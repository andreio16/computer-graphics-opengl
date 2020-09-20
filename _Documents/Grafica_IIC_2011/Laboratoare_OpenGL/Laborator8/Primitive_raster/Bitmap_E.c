/*
 *  Programul este un exemplu de utilizare a primitivei bitmap
 *  in OpenGL, pentru desenarea fonturilor.
 */
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
// tabloul care conþine litera E, 16X12 biþi
GLubyte litera_E[24] = {
    0xff, 0xc0, 0xff, 0xc0, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00,
    0xff, 0xc0, 0xff, 0xc0};

void CALLBACK display(void)
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (0.0, 0.0, 0.0);

    glRasterPos2i (-2.0, 0.0); //pozitia curentã pentru primul bitmap
	// datã în unitãti logice (sunt 10 unitãþi pe axa x în intervalul(-5, 5))
    glBitmap (10, 12, 0.0, 0.0, 20.0, 0.0, litera_E); // deplasarea este datã
	//în pixeli;  20 de pixeli pe axa x
	// 
    glBitmap (10, 12, 0.0, 0.0, 20.0, 0.0, litera_E);
    glBitmap (10, 12, 0.0, 0.0, 20.0, 0.0, litera_E);
	glBitmap (10, 12, 0.0, 0.0, 20.0, 0.0, litera_E);
    glFlush();
}
// proiectie ortogonalã
//se mapeazã 10 unitãti pe fiecare axã, o unitate are 20 pixeli
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
 if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
    glOrtho (-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
        5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
    else 
    glOrtho (-5.0*(GLfloat)w/(GLfloat)h, 
        5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 200, 200);
    auxInitWindow ("Bitmap_E");
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
