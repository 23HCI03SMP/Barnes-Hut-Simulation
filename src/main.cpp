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

    for (int i = 0; i <= iterations; i++)
    {
        // Start timer
        auto start = high_resolution_clock::now();

        // Get all particles in octree
        std::vector<Octree *> childVect = getChildren(final);

        Barnes barnes;
        for (Octree *child : childVect)
        {
            // calculate and update forces/fields
            barnes.calcForce(final, child, theta);
        }

        // Stop timer
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << std::endl;

        // Update positions and velocities
        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        writeSimulationValues(final, ValueFile);
    }

    ValueFile.close();
    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;
}

int main()
{
    // Define particles in plasma
    std::vector<InsertedParticle> particles = {
        InsertedParticle("Deutron", 2, 1, 0.5),
        InsertedParticle("Electron", 1 / 1823.0f, -1, 0.5),
    };

    // Create Octree object
    Octree tree = Octree(0, 0, 0, 20, 20, 20);
    Octree *tree_ptr = &tree;

    // std::vector<CSVPoint> points = generateInitialPoints(tree_ptr, 1000, 294, particles, Shape::SPHERE, {2}); // 293K = 20C
    std::vector<CSVPoint> points = generateInitialPoints(tree_ptr, 20, 294, particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}); // 293K = 20C

    // Start simulation loop
    loop(tree_ptr, 50, 0, 1e-8);

    // Animate and generate .vtk files
    system("py ./animator.py");
    system("py ./vtk_animator.py");

    return 0;
}
