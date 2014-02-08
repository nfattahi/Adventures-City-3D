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
 * CityLoader.cpp
 */

#include "CityLoader.h"

void CityLoader::parseCity()
{
    int currElement = 0;
    BuildingData data;
    FILE * file = fopen(this->cityfile, "r");
    
    /* Go through the city file */
    while (true)
    {
        char textline[256];     // buffer to hold the line content
        
        // Catch end of file
        if (fscanf(file, "%s", textline) == EOF)
        {
            break;
        }
        
        // For lines that are not comments...
        else if (textline[0] != '#')
        {
            string path(textline);
            Building building;
            float num = atof(textline);

            // Get position, rotation and scale factors out of current line
            if(currElement == 0)
            {
                data.addr = path;
                currElement++;
            }
            else if (currElement == 1)
            {
                data.xpos = num;
                currElement++;
            }
            else if (currElement == 2)
            {
                data.ypos = num;
                currElement++;
            }
            else if (currElement == 3)
            {
                data.zpos = num;
                currElement++;
            }
            else if (currElement == 4)
            {
                data.xrot = num;
                currElement++;
            }
            else if (currElement == 5)
            {
                data.yrot = num;
                currElement++;
            }
            else if (currElement == 6)
            {
                data.zrot = num;
                currElement++;
            }
            else if (currElement == 7)
            {
                data.xscl = num;
                currElement++;
            }
            else if (currElement == 8)
            {
                data.yscl = num;
                currElement++;
            }
            else if (currElement == 9)
            {
                data.zscl = num;
                
                if (strncmp (data.addr.c_str(),"0", 1) != 0) {
                    building.parseBuilding(data.addr.c_str());
                    building.setTrans(data.xpos, data.ypos, data.zpos);
                    building.setRotat(data.xrot, data.yrot, data.zrot);
                    building.setScale(data.xscl, data.yscl, data.zscl);
                    
                    if (buildings.size() != 0)
                    {
                        building.setTexCount(buildings.back().getTextureSize());
                    }
                    buildings.push_back(building);
                    currElement = 0;
                }
            }
        }
    }
}

void CityLoader::renderCity()
{
    int counter = 0;
    while (counter < buildings.size())
    {
        buildings[counter].renderBuilding();
        counter++;
    }
}