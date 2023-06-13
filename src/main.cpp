#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

void loop(Octree *octree, int iterations, float theta, float timeStep)
{
    Octree *final = octree;

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

        // Update simulation_values.csv file
        generateSimulationValuesFile(final);
    }

    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;
}

int main()
{
    // Define particles in plasma
    std::vector<InsertedParticle> particles = {
        InsertedParticle("Deutron", 2, 1, 0.5),
        InsertedParticle("Electron", 1/1823.0f, -1, 0.5),
    };

    // Create Octree object
    Octree tree = Octree(0, 0, 0, 20, 20, 20);
    Octree *tree_ptr = &tree;

    // Generate initial points
    //std::vector<CSVPoint> points = generateInitialPoints(tree_ptr, 2, 2, 10, 100, 294, particles, Shape::REGULAR_CYLINDER); // 293K = 20C
    tree.insert(tree_ptr, "Deutron", 1, 1, 1, 1, 1, 1, 1, 1);

    // Start simulation loop
    loop(tree_ptr, 50, 0, 1e10);

    // Animate and generate .vtk files
    system("py ./animator.py");
    system("py ./vtk_animator.py");

    return 0;
}
