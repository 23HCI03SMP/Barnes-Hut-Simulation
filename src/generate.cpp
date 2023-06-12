#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void writeSimulationValues(Octree *octree, std::ofstream &ValueFile)
{
    std::vector<Octree *> children = getChildren(octree);

    for (Octree *child : children)
    {
        std::string alias = child->alias;
        float x = child->point->x;
        float y = child->point->y;
        float z = child->point->z;
        float vx = child->velocityX;
        float vy = child->velocityY;
        float vz = child->velocityZ;
        float mass = child->mass;
        float charge = child->charge;

        ValueFile
            << "\n"
            << x << ","
            << y << ","
            << z << ","
            << vx << ","
            << vy << ","
            << vz << ","
            << mass << ","
            << charge << ","
            << alias;
    }

    ValueFile << "\n";
}