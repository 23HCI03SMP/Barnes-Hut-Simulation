#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"

std::vector<std::array<float, 4>> generatePoints(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float mass, float density)
{
    if (minX <= 0 || minY <= 0 || minZ <= 0)
    {
        throw std::invalid_argument("Coordinates cannot be negative");
    }

    std::vector<std::array<float, 4>> points;
    float phi, theta;

    for (phi = 0; phi <= PI; phi += density) {
        for (theta = 0; theta <= 2 * PI; theta += density) {
            float x = sin(phi) * cos(theta) * (maxX - minX) / 2 + (maxX + minX) / 2;
            float y = sin(phi) * sin(theta) * (maxY - minY) / 2 + (maxY + minY) / 2;
            float z = cos(phi) * (maxZ - minZ) / 2 + (maxZ + minZ) / 2;

            points.push_back({x, y, z, mass});
        }
    }

    return points;
}