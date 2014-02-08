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
 * CityLoader.h
 */

#ifndef _CITY_H_
#define _CITY_H_

#include <vector>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Building.h"

struct BuildingData
{
    float xpos, ypos, zpos;
    float xrot, yrot, zrot;
    float xscl, yscl, zscl;
    string addr;
};

class CityLoader
{
public:
    char* cityfile;
    void parseCity();
    void renderCity();
    
private:
    vector<Building> buildings;
};

#endif