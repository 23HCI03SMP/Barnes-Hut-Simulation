#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

// Overload to generate a specific number of particles, rather than a specific density
std::vector<CSVPoint> generateInitialPonts(float minX, float minY, float minZ,
                                           float maxX, float maxY, float maxZ,
                                           float radius,
                                           int particleNumber,
                                           float temperature,
                                           std::vector<Particle> particles)
{
    float density = std::ceil(particleNumber / (4.0f * PI * std::pow(radius, 3.0f) / 3.0f));

    return generateInitialPoints(minX, minY, minZ, maxX, maxY, maxZ, radius, density, temperature, particles);
}

std::vector<CSVPoint> generateInitialPoints(float minX, float minY, float minZ,
                                            float maxX, float maxY, float maxZ,
                                            float radius,
                                            float density,
                                            float temperature,
                                            std::vector<Particle> particles)
{
    std::vector<CSVPoint> points;

    float mass = 1;

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    float rangeZ = maxZ - minZ;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));

    int n = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

    gsl_rng *rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    for (int i = 0; i < n; ++i)
    {
        float x, y, z, vx, vy, vz;
        do
        {
            // generate random coordinates within the sphere
            x = gsl_ran_gaussian(rng, radius) + maxX / 2.0;
            y = gsl_ran_gaussian(rng, radius) + maxY / 2.0;
            z = gsl_ran_gaussian(rng, radius) + maxZ / 2.0;
        } while (pow(x - (minX + maxX) / 2, 2) + pow(y - (minY + maxY) / 2, 2) + pow(z - (minZ + maxZ) / 2, 2) > pow(radius, 2)); // ensure coordinates are within range

        // generate random velocities for the point using Maxwell-Boltzmann distribution
        vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
        vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
        vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);

        points.push_back(CSVPoint(x, y, z, vx, vy, vz, mass, 1.0f));
    }

    gsl_rng_free(rng);

    return points;
}