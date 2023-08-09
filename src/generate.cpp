#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void clearSimulationValues()
{
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios::trunc);

    ValueFile << VALUE_FILE_HEADER;
    ValueFile.close();
}

void writeSimulationValues(std::vector<Octree *> children, std::ofstream &ValueFile)
{
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