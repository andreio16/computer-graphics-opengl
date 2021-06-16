#include "GLee.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtx/component_wise.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include "gltools.h"	// OpenGL toolkit
#include <math.h>
#include "math3d.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

struct VertRef
{
    VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
    int v, vt, vn;
};

std::vector< Vertex > LoadOBJ(std::istream& in)
{
    std::vector< Vertex > verts;

    std::vector< glm::vec4 > positions(1, glm::vec4(0, 0, 0, 0));
    std::vector< glm::vec3 > texcoords(1, glm::vec3(0, 0, 0));
    std::vector< glm::vec3 > normals(1, glm::vec3(0, 0, 0));
    std::string lineStr;
    while (std::getline(in, lineStr))
    {
        std::istringstream lineSS(lineStr);
        std::string lineType;
        lineSS >> lineType;

        // vertex
        if (lineType == "v")
        {
            float x = 0, y = 0, z = 0, w = 1;
            lineSS >> x >> y >> z >> w;
            positions.push_back(glm::vec4(x, y, z, w));
        }

        // texture
        if (lineType == "vt")
        {
            float u = 0, v = 0, w = 0;
            lineSS >> u >> v >> w;
            texcoords.push_back(glm::vec3(u, v, w));
        }

        // normal
        if (lineType == "vn")
        {
            float i = 0, j = 0, k = 0;
            lineSS >> i >> j >> k;
            normals.push_back(glm::normalize(glm::vec3(i, j, k)));
        }

        // polygon
        if (lineType == "f")
        {
            std::vector< VertRef > refs;
            std::string refStr;
            while (lineSS >> refStr)
            {
                std::istringstream ref(refStr);
                std::string vStr, vtStr, vnStr;
                std::getline(ref, vStr, '/');
                std::getline(ref, vtStr, '/');
                std::getline(ref, vnStr, '/');
                int v = atoi(vStr.c_str());
                int vt = atoi(vtStr.c_str());
                int vn = atoi(vnStr.c_str());
                v = (v >= 0 ? v : positions.size() + v);
                vt = (vt >= 0 ? vt : texcoords.size() + vt);
                vn = (vn >= 0 ? vn : normals.size() + vn);
                refs.push_back(VertRef(v, vt, vn));
            }

            // triangulate, assuming n>3-gons are convex and coplanar
            for (size_t i = 1; i + 1 < refs.size(); ++i)
            {
                const VertRef* p[3] = { &refs[0], &refs[i], &refs[i + 1] };

                glm::vec3 U(positions[p[1]->v] - positions[p[0]->v]);
                glm::vec3 V(positions[p[2]->v] - positions[p[0]->v]);
                glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));

                for (size_t j = 0; j < 3; ++j)
                {
                    Vertex vert;
                    vert.position = glm::vec3(positions[p[j]->v]);
                    vert.texcoord = glm::vec2(texcoords[p[j]->vt]);
                    vert.normal = (p[j]->vn != 0 ? normals[p[j]->vn] : faceNormal);
                    verts.push_back(vert);
                }
            }
        }
    }

    return verts;
}

int btn;
glm::ivec2 startMouse;
glm::ivec2 startRot, curRot;
glm::ivec2 startTrans, curTrans;
const char* szCubeFaces[6] = { "pos_x.tga", "neg_x.tga", "pos_y.tga", "neg_y.tga", "pos_z.tga", "neg_z.tga" };

GLuint      textureObjects[3];
#define CUBE_MAP        0

GLenum  cube[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
std::vector< Vertex > model;



void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        btn = button;
        startMouse = glm::ivec2(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
        startRot = curRot;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        btn = button;
        startMouse = glm::ivec2(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
        startTrans = curTrans;
    }
}

void DrawSkyBox(void)
{
    GLfloat fExtent = 50.0f;

    glBegin(GL_QUADS);
    //////////////////////////////////////////////
    // Negative X
    // Note, we must now use the multi-texture version of glTexCoord
    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
    glVertex3f(-fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
    glVertex3f(-fExtent, -fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
    glVertex3f(-fExtent, fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
    glVertex3f(-fExtent, fExtent, fExtent);


    ///////////////////////////////////////////////
    //  Postive X
    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
    glVertex3f(fExtent, -fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
    glVertex3f(fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
    glVertex3f(fExtent, fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
    glVertex3f(fExtent, fExtent, -fExtent);


    ////////////////////////////////////////////////
    // Negative Z 
    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
    glVertex3f(-fExtent, -fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
    glVertex3f(fExtent, -fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
    glVertex3f(fExtent, fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
    glVertex3f(-fExtent, fExtent, -fExtent);


    ////////////////////////////////////////////////
    // Positive Z 
    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
    glVertex3f(fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
    glVertex3f(-fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
    glVertex3f(-fExtent, fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
    glVertex3f(fExtent, fExtent, fExtent);


    //////////////////////////////////////////////////
    // Positive Y
    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
    glVertex3f(-fExtent, fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
    glVertex3f(-fExtent, fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
    glVertex3f(fExtent, fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
    glVertex3f(fExtent, fExtent, fExtent);


    ///////////////////////////////////////////////////
    // Negative Y
    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
    glVertex3f(-fExtent, -fExtent, -fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
    glVertex3f(-fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
    glVertex3f(fExtent, -fExtent, fExtent);

    glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
    glVertex3f(fExtent, -fExtent, -fExtent);
    glEnd();
}

void DrawObjModel(GLubyte red, GLubyte green, GLubyte blue)
{

    glPushMatrix();
    {
        glColor3ub(red, green, blue);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glRotatef(0, 1, 0, 0);
        glRotatef(180, 0, 1, 0);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &model[0].position);
        glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &model[0].texcoord);
        glNormalPointer(GL_FLOAT, sizeof(Vertex), &model[0].normal);
        glDrawArrays(GL_TRIANGLES, 0, model.size());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    glPopMatrix();
}

void motion(int x, int y)
{
    glm::ivec2 curMouse(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
    if (btn == GLUT_LEFT_BUTTON)
    {
        curRot = startRot + (curMouse - startMouse);
    }
    else if (btn == GLUT_RIGHT_BUTTON)
    {
        curTrans = startTrans + (curMouse - startMouse);
    }
    glutPostRedisplay();
}

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    double ar = w / h;
    glTranslatef(curTrans.x / w * 2, curTrans.y / h * 2, 0);
    gluPerspective(60, ar, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -10);
    glRotatef(180, 0, 1, 0);

    glPushMatrix();
    {
        glRotatef(curRot.x % 360, 0, 1, 0);
        glRotatef(-curRot.y % 360, 1, 0, 0);

        // object
        DrawObjModel(149, 135, 115);

        //Sky Box is manually textured
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE1);

        glEnable(GL_TEXTURE_CUBE_MAP);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        DrawSkyBox();

        // Use texgen to apply cube map
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisable(GL_TEXTURE_CUBE_MAP);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glutSwapBuffers();
}

// return the min/max points of pts
template< typename Vec >
std::pair< Vec, Vec > GetExtents(const Vec* pts, size_t stride, size_t count)
{
    unsigned char* base = (unsigned char*)pts;
    Vec pmin(*(Vec*)base);
    Vec pmax(*(Vec*)base);
    for (size_t i = 0; i < count; ++i, base += stride)
    {
        const Vec& pt = *(Vec*)base;
        pmin = glm::min(pmin, pt);
        pmax = glm::max(pmax, pt);
    }

    return std::make_pair(pmin, pmax);
}

// centers geometry around the origin
// and scales it to fit in a size^3 box
template< typename Vec >
void CenterAndScale(Vec* pts, size_t stride, size_t count, const typename Vec::value_type& size)
{
    typedef typename Vec::value_type Scalar;

    // get min/max extents
    std::pair< Vec, Vec > exts = GetExtents(pts, stride, count);

    // center and scale 
    const Vec center = (exts.first * Scalar(0.5)) + (exts.second * Scalar(0.5f));

    const Scalar factor = size / glm::compMax(exts.second - exts.first);
    unsigned char* base = (unsigned char*)pts;
    for (size_t i = 0; i < count; ++i, base += stride)
    {
        Vec& pt = *(Vec*)base;
        pt = ((pt - center) * factor);
    }
}

void SetupRC()
{
    GLfloat fScale = 0.01f;
    GLbyte* pBytes;
    GLbyte* pBytesTexture;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    int i;


    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glGenTextures(2, textureObjects);

    // Set up texture maps   
    // Cube Map
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP]);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Load Cube Map images
    for (i = 0; i < 6; i++)
    {
        // Load this texture map
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
        pBytes = gltLoadTGA(szCubeFaces[i], &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    /////////////////////////////////////////////////////////////////////
    // Set up the texture units

    // First texture unit contains the color map
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   // Decal tarnish

    // Second texture unit contains the cube map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glEnable(GL_TEXTURE_CUBE_MAP);

    // Multiply this texture by the one underneath
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Swith to default texture


    // Load Thunderbird body and canopy
    // Temporary workspace
    M3DVector3f vVerts[3];
    M3DVector3f vNorms[3];
    M3DVector2f vTex[3];

    GLfloat lightPos[]   = { -40.0f, 100.0f, 100.0f, 1.0f };
    GLfloat fAmbLight[]  = { 0.075f, 0.075f, 0.075f, 0.0f };
    GLfloat fDiffLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat fSpecLight[] = { 0.1f, 0.1f, 0.1f, 0.0f };

    // Set up lighting
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fSpecLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecLight);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbLight);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Light never changes, put it here
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Load texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    pBytesTexture = gltLoadTGA(".\\camouflage.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytesTexture);
    free(pBytesTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

int main(int argc, char** argv)
{
    std::ifstream ifile("vw_van.obj");
    model = LoadOBJ(ifile);
    CenterAndScale(&model[0].position, sizeof(Vertex), model.size(), 7);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1440, 900);
    glutCreateWindow("Volkswagen Transporter - 3D Model");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glEnable(GL_DEPTH_TEST);

    // set up "headlamp"-like light
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat position[] = { 10, 10, 10, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);


    SetupRC();
    glutMainLoop();

    return 0;
}
