#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

using namespace std::chrono;

void loop(Octree *octree, int iterations, float theta, float timeStep)
{
    std::cout << "Starting simulation loop of theta of " << theta << " and time step of " << timeStep << " for " << iterations << " iterations" << std::endl;

    Octree *final = octree;
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios::app);

    float totalDur = 0;
    float totalProcessingDur = 0;
    float peSum = 0;

    // @note: Basically there's a change in how values are logged to the simulation_values.csv. We no longer
    //        log the values of the initial conditions, but rather the values of the simulation before each loop is performed.
    //        That way we avoid needing to run getChildren() multiple times
    for (int i = 0; i <= iterations; i++)
    {
        // Start timer
        auto start = high_resolution_clock::now();

        // Get all particles in octree
        std::vector<Octree *> children = getChildren(final);
        auto t1 = high_resolution_clock::now(); // Time taken to get children
        writeSimulationValues(children, ValueFile);
        auto t2 = high_resolution_clock::now(); // Time taken to write values to file

        Barnes barnes;
        for (Octree *child : children)
        {
            // calculate and update forces/fields
            barnes.calcForce(final, child, theta);
        }

        // calculate pe between children in a list of chidren
        // lop through children and calculate pe between each pair
        // add pe to total pe
        for (int i = 0; i < children.size(); i++)
        {
            for (int j = i + 1; j < children.size(); j++)
            {
                Octree *node = children[i];
                Octree *b = children[j];

                float distance = sqrt(pow(node->point->x - b->point->x, 2) + pow(node->point->y - b->point->y, 2) + pow(node->point->z - b->point->z, 2));
                float pe = (K_E * node->charge * b->charge) / distance;

                peSum += 2 * pe;
            }
        }

        // Update positions and velocities
        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        // Stop timer
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        auto processingDuration = duration_cast<milliseconds>(stop - t2) + duration_cast<milliseconds>(t1 - start);

        totalDur += duration.count();
        totalProcessingDur += processingDuration.count();

        std::cout << "Completed iteration " << i << " in " << duration.count() << "ms (" << processingDuration.count() << "ms processing)" << std::endl;

        // output pe to pe.txt
        std::ofstream peFile;
        peFile.open(std::filesystem::current_path() / "pe.txt", std::ios_base::app);
        peFile << "pe: " << peSum << std::endl;

        peSum = 0;
    }

    ValueFile.close();
    std::cout << "Average time: " << totalDur / iterations << "ms (" << totalProcessingDur / iterations << "ms processing)" << std::endl;
}

int main()
{
    // Define particles in plasma
    std::vector<InsertedParticle> fuel_particles = {
        InsertedParticle("Deuteron", 2, 1, 0.5),
        InsertedParticle("Electron", 1 / 1836.0f, -1, 0.5),
    };

    std::vector<InsertedParticle> test_particles = {
        InsertedParticle("Deuteron", 2 * 1836, 1, 0.5),
        InsertedParticle("Electron", 1, -1, 0.5),
    };

    std::vector<InsertedParticle> liner_particles = {
        InsertedParticle("Beryllium", 9, 2, 0.333),
        InsertedParticle("LinerElectron", 1 / 1836.0f, -1, 0.667)
    };

    std::filesystem::path filePath = std::filesystem::current_path() / "energy.txt";
    std::remove(filePath.string().c_str());

    // Create Octree object
    Octree tree = Octree(0, 0, 0, 100, 100, 100);
    Octree *tree_ptr = &tree;

    generateInitialPoints(tree_ptr, 50, 34800, fuel_particles, Shape::REGULAR_CYLINDER, {2, 10});               // Generate hot rod
    generateInitialPoints(tree_ptr, 100, 293, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true, true); // Generate liner
    // generateInitialPoints(tree_ptr, 5, 294, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true);
    // generateInitialPoints(tree_ptr, 100, 11000, fuel_particles, Shape::SPHERE, {4});
    // generateInitialPoints(tree_ptr, 100, 293, {InsertedParticle("Deuteron", 2, 1, 0.5)}, Shape::SPHERE, {4}); // Generate liner
    generateInitialPoints(tree_ptr, 50, 11.6e6, test_particles, Shape::SPHERE, {4}); 

    // tree_ptr->insert(tree_ptr, new Octree("Electron", 1, 1, 1, 0, 0, 0, 1, -1));
    // tree_ptr->insert(tree_ptr, new Octree("Electron", 10, 10, 10, 0, 0, 0, 1, -1));

    // Start simulation loop
    loop(tree_ptr, 50, 0, 1e3);

    // Animate and generate .vtk files
    system("py ./animator.py");
    // system("py ./vtk_animator.py");

    return 0;
}
