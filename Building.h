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
 * Building.h
 */

#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <string>
#include <vector>
#include <cerrno>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

struct CartesianHolder
{
    float x;
    float y;
    float z;
};

class Building
{
public:
    Building(): texCount(0) {}
    
    void parseBuilding(const char*);
    void renderBuilding();
    
    // Accessors
    int getTextureSize();
    
    // Mutators
    void setTrans(float x, float y, float z);
    void setRotat(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setTexCount(int num);
    
private:
    int texCount;
    vector<float> vertices;
    vector<float> triangles;
    vector<float> normals;
    vector<string> textures;
    vector<GLuint> texIDs;
    string root;
    GLuint texID;
    CartesianHolder trans;
    CartesianHolder rotat;
    CartesianHolder scale;
    
    void loadPPM();
    void putTexture();
};

#endif