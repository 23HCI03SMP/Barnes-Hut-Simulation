#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

Octree loop(Octree* octree, int iterations, float theta, float timeStep)
{
    Octree final = Octree(0, 0, 0, 0, 0, 0);

    for (int i = 0; i < iterations; i++)
    {
        std::vector<Octree *> childVect = getNodes(octree);
        Barnes barnes;

        for (Octree *child : childVect)
        {
            barnes.calcForce(octree, child, theta);
        }

        Simulation sim = Simulation();
        final = sim.mainLoop(octree, 1, timeStep);

        generateSimulationValuesFile(&final);
    }

    return final;
}

int main()
{
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 1, 1, 200, 293); // 293K = 20C
    generateInitialValuesFile(points);

    std::vector<CSVPoint> initialPoints = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    loadAndInsertInitialValues(tree_ptr);

    initialiseSimulationValuesFile(initialPoints);

    auto start = high_resolution_clock::now();
    Octree final = loop(tree_ptr, 200, 0, 1e-10); 

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << " ms " << duration.count()/1000 << " s" << std::endl;

    std::getchar();
    return 0;
}