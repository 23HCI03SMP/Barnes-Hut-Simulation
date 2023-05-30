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

    // Loop through each line, and then float parse each value in the line
    std::string line;
    std::array<float, 8> values;
    while (std::getline(ValueString, line))
    {
        std::istringstream s(line);
        std::string field;
        int i = 0;

        while (std::getline(s, field, ','))
        {
            values[i] = std::stof(field);
            i++;
        }

        valuesVect.push_back(CSVPoint(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]));
    }

    ValueString.close();

    return valuesVect;
}