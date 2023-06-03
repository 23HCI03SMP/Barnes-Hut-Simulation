#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

#include "testing.cpp"

using namespace std::chrono;

Octree loop(Octree *octree, int iterations, float theta, float timeStep)
{
    // Octree *octree = &octreeM;

    Octree final = Octree(0, 0, 0, 0, 0, 0);

    float totalDur = 0;

    for (int i = 0; i < iterations; i++)
    {
        auto start = high_resolution_clock::now();
        std::vector<Octree *> childVect = getChildren(octree);
        Barnes barnes;

        for (Octree *child : childVect)
        {
            barnes.calcForce(octree, child, theta);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << std::endl;

        Simulation sim = Simulation();
        final = sim.mainLoop(octree, 1, timeStep);

        generateSimulationValuesFile(&final);
    }

    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;

    return final;
}

int main()
{
    std::string fileName = "simulation_values.csv";
    remove(fileName.c_str());

    std::cout << "Starting simulation..." << std::endl;
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 2, 1, 1, 293); // 293K = 20C
    generateInitialValuesFile(points);

    std::vector<CSVPoint> initialPoints = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    int i = 0;
    for (CSVPoint point : initialPoints)
    {
        tree.insert(
            tree_ptr,
            point.x,
            point.y,
            point.z,
            point.vx,
            point.vy,
            point.vz,
            point.mass,
            point.charge);
        // std::cout << "Inserted " << i << " points\n";
        i++;
    } 

    // tree.insert(tree_ptr, 2.97642,3.64714,4.88192,-1.00691e-11,-6.63583e-11,-8.98e-11,1,1);
    // tree.insert(tree_ptr, 1.66495,1.97908,3.26532,-1.1901e-10,-4.59612e-11,7.26098e-11,1,1);
    // tree.insert(tree_ptr, 3.3714,2.55835,4.31022,5.11245e-11,-4.65457e-11,4.13986e-11,1,1);
    // tree.insert(tree_ptr, 2.67635,3.67637,2.74369,-3.11227e-11,1.03224e-11,6.52912e-11,1,1);
    // tree.insert(tree_ptr, 2.85518,1.77497,3.70726,1.29438e-12,-2.01606e-11,-3.98617e-11,1,1);

    tree_ptr->recalculateCenterOfCharge(tree_ptr);

    // initialiseSimulationValuesFile(initialPoints);
    Octree final = loop(tree_ptr, 100, 0, 1e-2);

    std::cout << "\nAnimator Starting...\n";
    system("py ./animator.py");

    return 0;
}