#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void generateFile(std::vector<CSVPoint> points)
{
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass,charge" << std::endl; // Insert headers into csv file

    for (CSVPoint point : points)
    {
        ValueFile
            << point.x << ","
            << point.y << ","
            << point.z << ","
            << point.vx << ","
            << point.vy << ","
            << point.vz << ","
            << point.mass << ","
            << point.charge
            << std::endl;
    }

    ValueFile.close();
}