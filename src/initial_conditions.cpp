#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

std::vector<std::array<float, 7>> generatePoints(
    float minX, float minY, float minZ, 
    float maxX, float maxY, float maxZ, 
    float mass, 
    float density,
    float temperature)
{
    std::vector<std::array<float, 7>> points;

    float sigma = 1.0f;  // standard deviation of gaussian distribution

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    float rangeZ = maxZ - minZ;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));

    float radius = maxRange / 2.0f;
    int n = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

    gsl_rng* rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    for (int i = 0; i < n; ++i)
    {
        float x, y, z, vx, vy, vz;
        do {
            // generate random coordinates within the sphere
            x = gsl_ran_gaussian(rng, radius) + maxX / 2.0;
            y = gsl_ran_gaussian(rng, radius) + maxY / 2.0;
            z = gsl_ran_gaussian(rng, radius) + maxZ / 2.0;
        } while (x < minX || x > maxX || y < minY || y > maxY || z < minZ || z > maxZ); // ensure coordinates are within range

        // generate random velocities for the point using Maxwell-Boltzmann distribution
        vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(2.0);
        vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(2.0);
        vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(2.0);

        points.push_back({x, y, z, vx, vy, vz, mass});
    }

    gsl_rng_free(rng);

    return points;
}