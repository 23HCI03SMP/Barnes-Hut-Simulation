#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

#include "testing.cpp"

using namespace std::chrono;

void loop(Octree *octree, int iterations, float theta, float timeStep)
{
    Octree *final = octree;

    float totalDur = 0;

    for (int i = 0; i < iterations; i++)
    {
        auto start = high_resolution_clock::now();
        std::vector<Octree *> childVect = getChildren(final);
        Barnes barnes;

        for (Octree *child : childVect)
        {
            barnes.calcForce(final, child, theta);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << std::endl;

        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        generateSimulationValuesFile(final);
    }

    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;
}

int main()
{
    std::string fileName = "simulation_values.csv";
    remove(fileName.c_str());

    std::cout << "Starting simulation..." << std::endl;
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 2, 2, 2, 1, 100, 293, Shape::SPHERE); // 293K = 20C
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

    tree_ptr->recalculateCenterOfCharge(tree_ptr);

    initialiseSimulationValuesFile(initialPoints);
    loop(tree_ptr, 100, 0, 1e-8);

    std::cout << "\nAnimator Starting...\n";
    system("py ./animator.py");

    return 0;
}