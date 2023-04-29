#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void generateFile(std::vector<std::array<float, 7>> points)
{   
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    ValueFile << "x,y,z,vx,vy,vz,mass" << std::endl; // Insert headers into csv file

    for (std::array<float, 7> point: points)
    {
        ValueFile 
            << point[0] << "," 
            << point[1] << "," 
            << point[2] << "," 
            << point[3] << "," 
            << point[4] << "," 
            << point[5] << ","
            << point[6]
            << std::endl;
    }

    ValueFile.close();
}