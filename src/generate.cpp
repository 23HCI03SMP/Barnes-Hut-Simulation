#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

/*
This generates both the initial values files as well as initialises the simulation values file.
That way, we do not need to load and inset the initial values separately.
*/
void generateFiles(std::vector<CSVPoint> points)
{
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass,charge,alias"; // Insert headers into csv file

    for (int i = 0; i < points.size(); i++)
    {
        CSVPoint point = points[i];
        
        ValueFile
            << "\n"
            << point.x << ","
            << point.y << ","
            << point.z << ","
            << point.vx << ","
            << point.vy << ","
            << point.vz << ","
            << point.mass << ","
            << point.charge << ","
            << point.alias;
    }

    ValueFile.close();

    initialiseSimulationValuesFile(points);
}

void initialiseSimulationValuesFile(std::vector<CSVPoint> initialPoints)
{
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass,charge,alias"; // Insert headers into csv file
    
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
            << point.charge << ","
            << point.alias;
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
        float mass = child->mass;
        float charge = child->charge;
        std::string alias = child->alias;

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
    ValueFile.close();
}