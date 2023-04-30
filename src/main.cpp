#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

Octree loop(Octree *octree, int iterations, float theta, float timeStep)
{
    Octree final = Octree(0, 0, 0, 0, 0, 0);

    for (int i = 0; i < iterations; i++)
    {
        std::vector<Octree *> childVect = getChildren(octree);
        Barnes barnes;

        for (Octree *&child : childVect)
        {
            for (Octree *&child2 : childVect)
            {
                if (child != child2)
                    barnes.calcForce(child, child2, theta);
            }
        }

        Simulation sim = Simulation();
        final = sim.mainLoop(octree, 1, timeStep);

        generateSimulationValuesFile(&final);
    }

    return final;
}

int main()
{
    auto start = high_resolution_clock::now();
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 1, 3, 293); // 293K = 20C
    generateInitialValuesFile(points);

    std::vector<CSVPoint> initialPoints = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

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
    }

    initialiseSimulationValuesFile();
    Octree final = loop(tree_ptr, 1, 0.1, 1e-10);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() << " µs " << duration.count()/1000 << " ms" << std::endl;

    std::getchar();
    return 0;
}