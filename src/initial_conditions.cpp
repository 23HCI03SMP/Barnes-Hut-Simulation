#include <cmath>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fstream>
#include <filesystem>

/*
Automatically loads to file and octree by default. To disable, set load to false.

For parameters length, breadth and height:

If shape is SPHERE, length is the diameter of the sphere. Breadth and height are supposed to be the same as length.
If shape is REGULAR_CYLINDER, length is the diameter of the cylinder. Breadth is the same as length. Height is the height of the cylinder.
*/
std::vector<CSVPoint> generateInitialPoints(
    Octree *&octree,
    float minX, float minY, float minZ,
    float maxX, float maxY, float maxZ,
    float length,
    float breadth,
    float height,
    float density,
    float temperature,
    std::vector<InsertedParticle> particles,
    Shape shape,
    bool load)
{
    std::vector<CSVPoint> points;
    std::ofstream ValueFile(std::filesystem::current_path() / INITIAL_VALUES_PATH);
    // std::ofstream SimulationFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH);

    if (load)
    {
        ValueFile << "x,y,z,vx,vy,vz,mass,charge,alias"; // Insert headers into csv file
    }

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    int nTotal;
    float x, y, z, vx, vy, vz;

    gsl_rng *rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    switch (shape)
    {
    case Shape::SPHERE:
    {
        if (length != breadth || length != height || breadth != height)
            throw std::invalid_argument("Length, breadth and height must be equal for a sphere");

        float radius = length / 2.0f;
        nTotal = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

        for (InsertedParticle particle : particles)
        {
            float nType = particle.percentage * nTotal;

            for (int i = 0; i < nType; ++i)
            {
                do
                {
                    // generate random coordinates within the sphere
                    x = gsl_ran_gaussian(rng, radius) + centerX;
                    y = gsl_ran_gaussian(rng, radius) + centerY;
                    z = gsl_ran_gaussian(rng, radius) + centerZ;
                } while (pow(x - centerX, 2) + pow(y - centerY, 2) + pow(z - centerZ, 2) > pow(radius * 2, 2)); // ensure coordinates are within range

                // generate random velocities for the point using Maxwell-Boltzmann distribution
                vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);

                points.push_back(CSVPoint(particle.alias, x, y, z, vx, vy, vz, particle.mass, 1.0f));

                if (load)
                {
                    ValueFile
                        << "\n"
                        << x << ","
                        << y << ","
                        << z << ","
                        << vx << ","
                        << vy << ","
                        << vz << ","
                        << particle.mass << ","
                        << particle.charge << ","
                        << particle.alias;

                    octree->insert(octree, particle.alias, x, y, z, vx, vy, vz, particle.mass, particle.charge);
                }
            }
        }

        break;
    }

    case Shape::REGULAR_CYLINDER:
    {
        throw std::runtime_error("Regular cylinder not implemented yet");

        if (length != breadth)
            throw std::invalid_argument("Length and breadth must be equal for a regular cylinder");

        float radius = length / 2.0f;
        nTotal = std::ceil(density * PI * radius * radius * height);

        for (InsertedParticle particle : particles)
        {
            float nType = particle.percentage * nTotal;

            // with reference to the sphere code above, generate a cylinder in the same fashion
            // except that the z-coordinate is limited to the height of the cylinder
            for (int i = 0; i < nType; ++i)
            {
                do
                {
                    // generate random coordinates within the cylinder
                    x = gsl_ran_gaussian(rng, radius) + maxX / 2.0;
                    y = gsl_ran_gaussian(rng, radius) + maxY / 2.0;
                    z = gsl_ran_gaussian(rng, height) + maxZ / 2.0;
                } while (pow(x - (minX + maxX) / 2, 2) + pow(y - (minY + maxY) / 2, 2) > pow(radius, 2) || (z < minZ || z > maxZ)); // ensure coordinates are within range

                // generate random velocities for the point using Maxwell-Boltzmann distribution
                vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);

                points.push_back(CSVPoint(particle.alias, x, y, z, vx, vy, vz, particle.mass, 1.0f));

                if (load)
                {
                    ValueFile
                        << "\n"
                        << particle.alias << ","
                        << x << ","
                        << y << ","
                        << z << ","
                        << vx << ","
                        << vy << ","
                        << vz << ","
                        << particle.mass << ","
                        << particle.charge;

                    octree->insert(octree, particle.alias, x, y, z, vx, vy, vz, particle.mass, particle.charge);
                }
            }
        }

        // for (int i = 0; i < nTotal; ++i)
        // {
        //     do
        //     {
        //         // generate random coordinates within the cylinder
        //         x = gsl_ran_gaussian(rng, radius) + maxX / 2.0;
        //         float angle = gsl_rng_uniform(rng) * 2 * PI;                 // random angle between 0 and 2π
        //         float distance = gsl_rng_uniform(rng) * (maxZ - minZ) / 2.0; // random distance along the z-axis within the cylinder height
        //         y = sin(angle) * radius + maxY / 2.0;
        //         z = cos(angle) * radius + minZ + distance;
        //     } while (pow(x - (minX + maxX) / 2, 2) + pow(y - (minY + maxY) / 2, 2) > pow(radius, 2) || (z < minZ || z > maxZ)); // ensure coordinates are within range

        //     // generate random velocities for the point using Maxwell-Boltzmann distribution
        //     vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
        //     vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);
        //     vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(mass);

        //     points.push_back(CSVPoint(x, y, z, vx, vy, vz, mass, 1.0f));
        // }

        break;
    }
    default:
        break;
    }

    octree->recalculateCenterOfCharge(octree);
    gsl_rng_free(rng);

    ValueFile.close();

    // Copy over values
    std::filesystem::remove(std::filesystem::current_path() / SIMULATION_VALUES_PATH);
    std::filesystem::copy_file(std::filesystem::current_path() / INITIAL_VALUES_PATH,
                               std::filesystem::current_path() / SIMULATION_VALUES_PATH,
                               std::filesystem::copy_options::overwrite_existing);

    // SimulationFile.close();

    return points;
}