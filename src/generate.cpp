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
        double x = child->point->x;
        double y = child->point->y;
        double z = child->point->z;
        double vx = child->velocityX;
        double vy = child->velocityY;
        double vz = child->velocityZ;
        double mass = child->mass;
        double charge = child->charge;

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