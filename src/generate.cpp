#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void generateFile(std::vector<std::array<float, 4>> points)
{   
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    ValueFile << "x,y,z,mass" << std::endl; // Insert headers into csv file

    for (std::array<float, 4> point: points)
    {
        ValueFile << point[0] << "," << point[1] << "," << point[2] << "," << point[3] << std::endl;
    }

    ValueFile.close();
}