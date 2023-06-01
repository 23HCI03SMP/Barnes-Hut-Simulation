#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/*
For parameters length, breadth and height:

If shape is SPHERE, length is the diameter of the sphere. Breadth and height are supposed to be the same as length.
If shape is REGULAR_CYLINDER, length is the diameter of the cylinder. Breadth is the same as length. Height is the height of the cylinder.
*/
std::vector<CSVPoint> generateInitialPoints(
    float minX, float minY, float minZ,
    float maxX, float maxY, float maxZ,
    float length,
    float breadth,
    float height,
    float mass,
    float density,
    float temperature,
    Shape shape)
{
    std::vector<CSVPoint> points;

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    float rangeZ = maxZ - minZ;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));

    int n;
    float x, y, z, vx, vy, vz;

    gsl_rng *rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    switch (shape)
    {
    case Shape::SPHERE:
    {
        if (length != breadth != height)
            throw std::invalid_argument("Length, breadth and height must be equal for a sphere");

        float radius = length / 2.0f;
        n = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

        for (int i = 0; i < n; ++i)
        {
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
        break;
    }

    case Shape::REGULAR_CYLINDER:
    {
        throw std::runtime_error("Regular cylinder not implemented yet");

        if (length != breadth)
            throw std::invalid_argument("Length and breadth must be equal for a regular cylinder");

        float radius = length / 2.0f;
        n = std::ceil(density * PI * radius * radius * height);

        for (int i = 0; i < n; ++i)
        {
            do
            {
                // generate random coordinates within the cylinder
                x = gsl_ran_gaussian(rng, radius) + maxX / 2.0;
                float angle = gsl_rng_uniform(rng) * 2 * PI;           // random angle between 0 and 2π
                float distance = gsl_rng_uniform(rng) * (maxZ - minZ)/ 2.0; // random distance along the z-axis within the cylinder height
                y = sin(angle) * radius + maxY / 2.0;
                z = cos(angle) * radius + minZ + distance;
            } while (pow(x - (minX + maxX) / 2, 2) + pow(y - (minY + maxY) / 2, 2) > pow(radius, 2) || (z < minZ || z > maxZ)); // ensure coordinates are within range

            // generate random velocities for the point using Maxwell-Boltzmann distribution
            vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
            vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
            vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);

            points.push_back(CSVPoint(x, y, z, vx, vy, vz, mass, 1.0f));
        }

        break;
    }
    default:
        break;
    }

    gsl_rng_free(rng);

    return points;
}