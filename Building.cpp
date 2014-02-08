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
 * Util.h
 *
 * With help from:
 *      http://netpbm.sourceforge.net/doc/ppm.html
 *      http://people.sc.fsu.edu/~jburkardt/cpp_src/ppmb_io/ppmb_io.html
 */

#include "Building.h"
#include "Util.h"

/* Parse Building model files */
void Building::parseBuilding(const char* path)
{
    // used to iterate through items on each line
    int paramCount = 0;
    bool verline = false;
    bool triline = false;
    bool norline = false;
    bool texline = false;
    string pathstr(path);
    string bName;
    
    // Extract building name from path (i.e. string before '/' character)
    int counter = 0;
    while( counter < pathstr.size())
    {
        char ch = pathstr[counter];
        if (ch == '/')
        {
            bName = pathstr.substr(0, counter);
            root += bName;
            root += '/';
        }
        counter++;
    }
    
    FILE * file = fopen(path, "r");    
    if( file == NULL )
    {
        printf("Model file: %s cannot be found!\n", path);
    }/*
    else
    {
        printf("Model File: %s\n", path);
    }*/
    char textline[256];
    
    while (fscanf(file, "%s", textline) != EOF)
    {
        if (verline)
        {
            float num = atof(textline);
            vertices.push_back(num);
            paramCount++;
            
            if (paramCount == 3) // each vertex has 3 parameters
            {
                verline = false;
                paramCount = 0;
            }
        }
        else if (triline)
        {
            float num = atof(textline);
            triangles.push_back(num);
            paramCount++;
            
            if (paramCount == 11) // each triangle has 11 parameters
            {
                triline = false;
                paramCount = 0;
            }
        }
        else if (norline)
        {
            float num = atof(textline);
            normals.push_back(num);
            paramCount++;
            
            if (paramCount == 3) // each normal has 3 parameters
            {
                norline = false;
                paramCount = 0;
            }
        }
        else if (texline)
        {
            string s(textline);
            string path = root;
            path += s;
            textures.push_back(path);
            texline = false;
            paramCount = 0;
        }

        // if it's a single character
        else if (textline[1] == '\0')
        {
            // it must mean that it's either a vertex or triangle
            if (textline[0] == 'v')
                verline = true;
            else if (textline[0] == 't')
                triline = true;
            else if (textline[0] == 'n')
                norline = true;
            else if (textline[0] == 'i')
                texline = true;
        }
    }
    loadPPM();
}

void Building::loadPPM()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    putTexture();
}

void Building::putTexture()
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        int width, height;
        unsigned char* imgppm;
        
        imgppm = Util::readppm(textures[i], width, height);
        
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width, height,0,GL_RGB,GL_UNSIGNED_BYTE,imgppm);
        
        texIDs.push_back(texID);
        
        delete[] imgppm;
    }
}

void Building::setTrans(float x, float y, float z)
{
    trans.x = x;trans.y = y;trans.z = z;
}

void Building::setRotat(float x, float y, float z)
{
    rotat.x = x;rotat.y = y;rotat.z = z;
}

void Building::setScale(float x, float y, float z)
{
    scale.x = x;scale.y = y;scale.z = z;
}

int Building::getTextureSize()
{
    int retval = texIDs.size() + texCount;
    return retval;
}

void Building::setTexCount(int val)
{
    texCount = val;
}

void Building::renderBuilding()
{
    glPushMatrix();
    
    // Scale, Translate, Rotate (note: inverse)
    glTranslatef(trans.x, trans.y, trans.z);
    glRotatef(rotat.x, 1, 0, 0);glRotatef(rotat.y, 0, 1, 0);glRotatef(rotat.z, 0, 0, 1);
    glScalef(scale.x, scale.y, scale.z);
    
    
    for (int i = 0; i < triangles.size(); i+=11)
    {
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, triangles[i+4] +texCount + 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glBegin(GL_TRIANGLES);

        int normal = triangles[i+3] * 3;        
        glNormal3f(normals[normal], normals[normal+1], normals[normal+2]);
        
        int v1 = triangles[i] * 3;
        int v2 = triangles[i+1] * 3;
        int v3 = triangles[i+2] * 3;
        
        glTexCoord2f(triangles[i+5], triangles[i+6]);
        glVertex3f(vertices[v1], vertices[v1 + 1], vertices[v1 + 2]);
        glTexCoord2f(triangles[i+7], triangles[i+8]);
        glVertex3f(vertices[v2], vertices[v2 + 1], vertices[v2 + 2]);
        glTexCoord2f(triangles[i+9], triangles[i+10]);
        glVertex3f(vertices[v3], vertices[v3 + 1], vertices[v3 + 2]);
        
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
        
    }
    
    glPopMatrix();
    
}
