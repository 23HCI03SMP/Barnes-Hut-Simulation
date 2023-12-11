#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>

std::vector<CSVPoint> loadInitialValues()
{
    std::vector<CSVPoint> valuesVect;
    std::ifstream ValueString(std::filesystem::current_path() / INITIAL_VALUES_PATH);

    // Read the first line to get the headers before starting to read values
    std::string headers;
    std::getline(ValueString, headers);

    // Loop through each line, and then double parse each value in the line
    std::string line;
    std::array<std::string, 9> values;
    while (std::getline(ValueString, line))
    {
        std::istringstream s(line);
        std::string field;
        int i = 0;

        while (std::getline(s, field, ','))
        {
            values[i] = field;
            i++;
        }

        double x = std::stof(values[0]);
        double y = std::stof(values[1]);
        double z = std::stof(values[2]);
        double vx = std::stof(values[3]);
        double vy = std::stof(values[4]);
        double vz = std::stof(values[5]);
        double mass = std::stof(values[6]);
        double charge = std::stof(values[7]);
        std::string alias = values[8];

        valuesVect.push_back(CSVPoint(alias, x, y, z, vx, vy, vz, mass, charge));
    }

    ValueString.close();

    return valuesVect;
}