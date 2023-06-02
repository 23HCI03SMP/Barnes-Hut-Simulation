#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void generateInitialValuesFile(std::vector<CSVPoint> points)
{
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass,charge"; // Insert headers into csv file

    for (CSVPoint point : points)
    {
        ValueFile
            << "\n"
            << point.x << ","
            << point.y << ","
            << point.z << ","
            << point.vx << ","
            << point.vy << ","
            << point.vz << ","
            << point.mass << ","
            << point.charge;
    }

    ValueFile.close();
}

void initialiseSimulationValuesFile(std::vector<CSVPoint> initialPoints)
{
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass,charge,fx,fy,fz"; // Insert headers into csv file
    
    for (CSVPoint point : initialPoints) {
        ValueFile
            << "\n"
            << point.x << ","
            << point.y << ","
            << point.z << ","
            << point.vx << ","
            << point.vy << ","
            << point.vz << ","
            << point.mass << ","
            << point.charge;
    }

    ValueFile << "\n";
    ValueFile.close();
}

void generateSimulationValuesFile(Octree *octree)
{
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios::app);

    std::vector<Octree *> children = getChildren(octree);

    for (Octree *child : children)
    {
        float x = child->point->x;
        float y = child->point->y;
        float z = child->point->z;
        float vx = child->velocityX;
        float vy = child->velocityY;
        float vz = child->velocityZ;
        float fx = child->forceX;
        float fy = child->forceY;
        float fz = child->forceZ;
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
            << fx << ","
            << fy << ","
            << fz;
    }

    ValueFile << "\n";
    ValueFile.close();
}