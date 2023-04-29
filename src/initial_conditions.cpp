#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

std::vector<std::array<float, 7>> generatePoints(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float mass, float density)
{
    std::vector<std::array<float, 7>> points;

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    float rangeZ = maxZ - minZ;
    float minRange = std::min(std::min(rangeX, rangeY), rangeZ);

    float radius = minRange / 2.0f;
    int numPoints = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

    gsl_rng* rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    for (int i = 0; i < numPoints; ++i)
    {
        float x, y, z;
        do {
            x = minX + gsl_rng_uniform(rng) * rangeX;
            y = minY + gsl_rng_uniform(rng) * rangeY;
            z = minZ + gsl_rng_uniform(rng) * rangeZ;
        } while (pow(x - 3, 2) + pow(y - 3, 2) + pow(z - 3, 2) > pow(radius, 2));

        float vel_scale = gsl_ran_gaussian(rng, 1.0f);
        float vx = vel_scale * gsl_ran_gaussian(rng, 1.0f);
        float vy = vel_scale * gsl_ran_gaussian(rng, 1.0f);
        float vz = vel_scale * gsl_ran_gaussian(rng, 1.0f);

        points.push_back({x, y, z, vx, vy, vz, mass});
    }

    gsl_rng_free(rng);

    return points;

    // if (minX <= 0 || minY <= 0 || minZ <= 0)
    // {
    //     throw std::invalid_argument("Coordinates cannot be negative");
    // }

    // std::vector<std::array<float, 4>> points;
    // float phi, theta;

    // for (phi = 0; phi <= PI; phi += density) {
    //     for (theta = 0; theta <= 2 * PI; theta += density) {
    //         float x = sin(phi) * cos(theta) * (maxX - minX) / 2 + (maxX + minX) / 2;
    //         float y = sin(phi) * sin(theta) * (maxY - minY) / 2 + (maxY + minY) / 2;
    //         float z = cos(phi) * (maxZ - minZ) / 2 + (maxZ + minZ) / 2;

    //         points.push_back({x, y, z, mass});
    //     }
    // }

    // return points;
}