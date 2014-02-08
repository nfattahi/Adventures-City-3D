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
 * Util.cpp
 *
 * With help from:
 *      http://netpbm.sourceforge.net/doc/ppm.html
 *      http://people.sc.fsu.edu/~jburkardt/cpp_src/ppmb_io/ppmb_io.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cerrno>

#include "Util.h"

namespace Util {
    
    unsigned char* readppm(const std::string &filename, int &width, int &height)
    {
        FILE *file;
        char buffer[256];
        int counter;
        int info[3];    // info[0]=width, info[1]=height, info[2]=maxval
        int tmp;
        
        file = fopen(filename.c_str(), "rb");
        if( !file )
        {
            printf("The error is - %s\n", strerror(errno));
            printf("File %s can't be openned!", filename.c_str());
        }
        
        counter = 0;
        fgets(buffer, 256, file);
        if( strncmp(buffer, "P6", 2) )
        {
            fclose(file);   // Not valid P6
        }
        
        while( (counter < 3) ) {
            if( (tmp=fgetc(file)) != '#' )
            {
                ungetc(tmp, file);
                fscanf(file, "%d", &info[counter++]);
            }
            else
            {
                fgets(buffer, 256, file);
            }
        }
        fgets(buffer, 256, file); // read to newline
        
        // Renew image
        width = info[0];
        height = info[1];
        unsigned char *img = new unsigned char[width*height*3];
        
        fread(img, 3, width * height, file);
        
        fclose(file);
        return img;
    }
    
    void writeppm(const std::string &filename, unsigned char *imgppm, int width, int height)
    {
        FILE *file;
        file = fopen(filename.c_str(), "wb");
        if( !file ) {
            printf("The error is - %s\n", strerror(errno));
            printf("File %s can't be openned!", filename.c_str());
        }
        
        fprintf(file, "P6\n%d %d\n255\n", width, height);
        fwrite(imgppm, 3, width * height, file);
        fclose(file);
    }
}
