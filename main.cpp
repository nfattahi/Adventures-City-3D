/*
 * EECE 478 - Assignment 3
 * 
 * Navid Fattahi
 * SID: 39937099
 * April 3, 2013
 * 
 * Development Environment:
 * XCode 4.4.1 - MAC OSX 10.7.5
 *
 * main.cpp
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <ctime>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Util.h"
#include "CityLoader.h"

using namespace std;

//Constants
const int width = 1280;
const int height = 720;
const float viewangle = 80;
const float zNear = 1;
const float zFar = 500;

int currWidth = width;
int currHeight = height;
int xRotation = 40;
int yRotation = -50;
float scale = 1;
float zTran = -45;
float xTran = -5;
float yTran = 0;
const int tranFactor = 5;

int frameCounter = 0;

static int frame = 0;
static int totalTime  = 0;
static int imgCounter = 0;

CityLoader* city = new CityLoader();

clock_t timer;
unsigned char * Pixel;

void DisplayText(char* str)
{
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();     //Push Projection Matrix
    
    glLoadIdentity();
    gluOrtho2D(0, currWidth, 0, currHeight);
    
    glColor3f (0.8,0.8,0.8);    // Text color
    glDisable(GL_LIGHTING);     // Disable lighting for text layer
    
    glRasterPos3f(currWidth - 120, 15, 1);
    
    for (str; *str != '\0'; str++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
    }
    
    glRasterPos3f(0, 15, 1);
    
    glPopMatrix ();
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix ();
    
    glEnable(GL_LIGHTING);  // Enable lighting back on
}

void Record()
{
        if (imgCounter % 1 == 0)
        {
            string filename = "frame";
            string imgnumstr;
            ostringstream imgnum;
            
            imgnum << imgCounter;
            imgnumstr = imgnum.str();
            
            filename += imgnumstr;
            filename += ".ppm";
            
            unsigned char * Pixel = new unsigned char[width * height * 3];
            glReadPixels(0, 0, width, width, GL_RGB, GL_UNSIGNED_BYTE, Pixel);
            Util::writeppm(filename, Pixel, width, height);
            delete Pixel;
        }
        imgCounter++;
    int t_elapsed = (clock() - timer) / (double)(CLOCKS_PER_SEC / 1000);
    
    if (t_elapsed > 1000)
    {
        frame = 0;
        timer = clock();
        totalTime++;
    }
    
    if (totalTime == 120)
    {
        exit(0);
    }
    else
    {
        cout << totalTime << endl;
    }
    
    frame++;
}

void moveCamera (int framecount)
{
    if (framecount < 25)
    {
        yRotation += 1;
        yTran += 0.5;
        yRotation += 0.1;
    }
    else if (framecount < 45)
    {
        yRotation += 0;
        xTran += 1;
        xRotation += -0.2;
    }
    else if (framecount < 110)
    {
        yRotation += 0;
        xTran += 1;
        yTran -= 1;
        xRotation += -0.5;
    }
    else if (framecount < 140)
    {
        yRotation -= 0.5;
        xTran += 1;
        zTran -= 1;
        yTran -= 0;
    }
    else if (framecount < 220)
    {
        xRotation -= 0.2;
        xTran += 0.5;
        zTran += 1;
        yTran -= 0;
    }
    else if (framecount < 280)
    {
        yRotation += 1;
    }
    else if (framecount < 350)
    {
        yTran += 0.5;
    }
    printf("framecounter = %d", framecount);
    frameCounter++;
}

/* OpenGL Display function */
void Display()
{
    Record();
    
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1, 1, 1);
    
    glLoadIdentity();
    
    moveCamera(frameCounter);
    
    glTranslatef(xTran, yTran, zTran);
    glScalef(scale, scale, scale);
    
    glRotatef(xRotation, 1, 0, 0);
    glRotatef(yRotation, 0, 1, 0);
        
    city->renderCity();
    DisplayText("Navid Fattahi");    
    
    glutSwapBuffers();
    
}

void Init ()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHT2 );
    glEnable( GL_LIGHT3 );
    glEnable( GL_LIGHT4 );
    glEnable( GL_LIGHT5 );
    glEnable( GL_LIGHT6 );
    glEnable( GL_LIGHT7 );

    GLfloat ambientColor[] = {0.2,0.2,0.2, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
}

/* OpenGL Reshape function:
   used when window is resized
 */
void Reshape(int w, int h)
{
    currWidth = w;
    currHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewangle, (float)w / (float)h, zNear, zFar);
}


// Keyboard function
void Keyboard ( unsigned char key, int mousex, int mousey )
{
    if (key == 'q')
        exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("No cityfile passed! Pass a cityfile to run the program.\n");
        printf("Usage: ./CityBuilder cityfile.model\n");
        return 0;
    }
    else
    {
        city->cityfile = argv[1];
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    glutInitWindowSize(width, height);
    glutCreateWindow("EECE 478 - Assignment 3 : UBCmini");
    
    city->parseCity();
    
    //light.init();
    timer = clock();
    
    glEnable(GL_CULL_FACE);
    glutDisplayFunc(Display);
    glutIdleFunc(Display);
    
    glutKeyboardFunc(Keyboard);
    
    glutReshapeFunc(Reshape);
    
    Init();
    
    glutMainLoop();

    return 0;
}