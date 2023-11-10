#include <cmath>
#include <cstdarg>
#include <stdexcept>
#include "include/barnesHut.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fstream>
#include <filesystem>

/**
 * @brief Generates initial points in a given shape and inserts them into the octree
 *
 * @param particles List of particles to be inserted into the octree
 * @param shape of the initial points
 * @param dimensions Dimensions of the shape. Pass in a certain number of values based on the shape paramater.
 * @param append Appends to the initial values file by default. Set to false by default.
 * @param load Automatically loads initial values to file and octree by default. Set to true by default.
 *
 * @note For a sphere, only one dimension is required. Pass in the radius as dimensions.
 * @note For a regular cylinder, two dimensions are required. Pass in the radius and height as dimensions.
 * @note For a holllow cylinder, three dimensions are required. Pass in the inner radius, outer radius, and height as dimensions.
 */
std::vector<CSVPoint> generateInitialPoints(
    Octree *&octree,
    float density,
    float temperature,
    std::vector<InsertedParticle> particles,
    Shape shape,
    std::initializer_list<float> dimensions,
    bool isLiner,
    bool append,
    bool load)
{
    float minX = octree->minPoints->x;
    float minY = octree->minPoints->y;
    float minZ = octree->minPoints->z;
    float maxX = octree->maxPoints->x;
    float maxY = octree->maxPoints->y;
    float maxZ = octree->maxPoints->z;

    std::vector<CSVPoint> points;
    std::ofstream InitialValueFile = append ? std::ofstream(std::filesystem::current_path() / INITIAL_VALUES_PATH, std::ios_base::app)
                                            : std::ofstream(std::filesystem::current_path() / INITIAL_VALUES_PATH, std::ios_base::trunc);
    std::ofstream SimulationValueFile = std::ofstream(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios_base::trunc);

    if (load && !append)
    {
        InitialValueFile << VALUE_FILE_HEADER; // Insert headers into csv file
    }

    SimulationValueFile << VALUE_FILE_HEADER; // Insert headers into csv file
    SimulationValueFile.close();

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
        float radius = *dimensions.begin();
        nTotal = std::ceil(density * 4.0f * PI * std::pow(radius, 3.0f) / 3.0f);

        for (InsertedParticle particle : particles)
        {
            float nType = particle.percentage * nTotal;

            for (int i = 0; i < nType; ++i)
            {
                do
                {
                    // generate random coordinates within the sphere
                    x = gsl_ran_flat(rng, -radius, radius) + centerX;
                    y = gsl_ran_flat(rng, -radius, radius) + centerY;
                    z = gsl_ran_flat(rng, -radius, radius) + centerZ;
                } while (pow(x - centerX, 2) + pow(y - centerY, 2) + pow(z - centerZ, 2) > pow(radius, 2)); // ensure coordinates are within range

                if (isLiner)
                {
                    vx = 0;
                    vy = 0;
                    vz = 0;
                }
                else
                {
                    // generate random velocities for the point using Maxwell-Boltzmann distribution
                    vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                }

                points.push_back(CSVPoint(particle.alias, x, y, z, vx, vy, vz, particle.mass, 1.0f));

                if (load)
                {
                    InitialValueFile
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
        float radius = *dimensions.begin();
        float height = *(dimensions.begin() + 1);

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
                    x = gsl_ran_flat(rng, -radius, radius) + centerX;
                    y = gsl_ran_flat(rng, -radius, radius) + centerY;
                    z = gsl_ran_flat(rng, -height / 2.0f, height / 2.0f) + centerZ;
                } while (pow(x - centerX, 2) + pow(y - centerY, 2) > pow(radius, 2)); // ensure coordinates are within range

                if(isLiner) {
                    vx = 0;
                    vy = 0;
                    vz = 0;
                } else {
                    // generate random velocities for the point using Maxwell-Boltzmann distribution
                    vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                }

                points.push_back(CSVPoint(particle.alias, x, y, z, vx, vy, vz, particle.mass, 1.0f));

                if (load)
                {
                    InitialValueFile
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

    case Shape::HOLLOW_CYLINDER:
    {
        float innerRadius = *dimensions.begin();
        float outerRadius = *(dimensions.begin() + 1);
        float height = *(dimensions.begin() + 2);

        nTotal = density * PI * (outerRadius * outerRadius - innerRadius * innerRadius) * height;

        for (InsertedParticle particle : particles)
        {
            float nType = particle.percentage * nTotal;

            // Generate particles inside the hollow cylinder
            for (int i = 0; i < nType; ++i)
            {
                do
                {
                    // Generate random coordinates within the hollow cylinder
                    float radius = gsl_ran_flat(rng, innerRadius, outerRadius);
                    float angle = gsl_ran_flat(rng, 0.0f, 2 * PI);
                    x = radius * std::cos(angle) + centerX;
                    y = radius * std::sin(angle) + centerY;
                    z = gsl_ran_flat(rng, -height / 2.0f, height / 2.0f) + centerZ;
                } while (pow(x - centerX, 2) + pow(y - centerY, 2) > pow(outerRadius, 2)); // Ensure coordinates are within the outer radius

                if (isLiner)
                {
                    vx = 0;
                    vy = 0;
                    vz = 0;
                }
                else
                {
                    // Generate random velocities for the point using Maxwell-Boltzmann distribution
                    vx = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vy = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                    vz = gsl_ran_gaussian(rng, sqrt(K_B * temperature)) / sqrt(particle.mass);
                }

                points.push_back(CSVPoint(particle.alias, x, y, z, vx, vy, vz, particle.mass, 1.0f));

                if (load)
                {
                    InitialValueFile
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
    }

    default:
        break;
    }

    octree->recalculateParentParameters(octree);
    gsl_rng_free(rng);

    InitialValueFile.close();

    std::cout << "Generated " << nTotal << " particles (Density " << density << ") at " << temperature << "K" << std::endl;

    return points;
}