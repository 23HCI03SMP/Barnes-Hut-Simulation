#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

using namespace std::chrono;

void loop(Octree *octree, int iterations, float theta, float timeStep)
{
    Octree *final = octree;
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios::app);

    float totalDur = 0;

    // @note: Basically there's a change in how values are logged to the simulation_values.csv. We no longer
    //        log the values of the initial conditions, but rather the values of the simulation before each loop is performed.
    //        That way we avoid needing to run getChildren() multiple times
    for (int i = 0; i <= iterations; i++)
    {
        // Start timer
        auto start = high_resolution_clock::now();

        // Get all particles in octree
        std::vector<Octree *> children = getChildren(final);
        writeSimulationValues(children, ValueFile);

        Barnes barnes;
        for (Octree *child : children)
        {
            // calculate and update forces/fields
            barnes.calcForce(final, child, theta);
        }

        // Update positions and velocities
        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        // Stop timer
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << std::endl;
    }

    ValueFile.close();
    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;
}

int main()
{
    // Define particles in plasma
    std::vector<InsertedParticle> fuel_particles = {
        InsertedParticle("Deuteron", 2, 1, 0.5),
        InsertedParticle("Electron", 1 / 1836.0f, -1, 0.5),
    };

    std::vector<InsertedParticle> liner_particles = {
        InsertedParticle("Beryllium", 9, 2, 0.333),
        InsertedParticle("LinerElectron", 1 / 1836.0f, -1, 0.667)
    };

    // Create Octree object
    Octree tree = Octree(0, 0, 0, 20, 20, 20);
    Octree *tree_ptr = &tree;

    generateInitialPoints(tree_ptr, 500, 293, fuel_particles, Shape::REGULAR_CYLINDER, {1, 8});               // Generate hot rod
    generateInitialPoints(tree_ptr, 100, 293, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true, true); // Generate liner
    // generateInitialPoints(tree_ptr, 20, 294, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true);
    // generateInitialPoints(tree_ptr, 20, 11000, fuel_particles, Shape::SPHERE, {4});

    // Start simulation loop
    loop(tree_ptr, 200, 0, 1e-9);

    // Animate and generate .vtk files
    system("py ./animator.py");
    // system("py ./vtk_animator.py");

    return 0;
}
