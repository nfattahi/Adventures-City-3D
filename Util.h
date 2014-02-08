#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace Util {
    
    unsigned char* readppm(const std::string &file, int &width, int &height);
    void writeppm(const std::string &file, unsigned char *ppm, int width, int height);
    
}

#endif
