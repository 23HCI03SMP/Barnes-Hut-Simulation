#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

#include "testing.cpp"

using namespace std::chrono;

Octree loop(Octree octreeM, int iterations, float theta, float timeStep)
{
    Octree *octree = &octreeM;

    Octree final = Octree(0, 0, 0, 0, 0, 0);

    float totalDur = 0;

    for (int i = 0; i < iterations; i++)
    {
        auto start = high_resolution_clock::now();
        std::vector<Octree *> childVect = getChildren(octree);
        Barnes barnes;

        for (Octree *&child : childVect)
        {
                barnes.calcForce(octree, child, theta);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << duration.count()/1000 << " s" << std::endl;

        Simulation sim = Simulation();
        final = sim.mainLoop(octree, 1, timeStep);

        generateSimulationValuesFile(&final);
    }

    std::cout << "Average time: " << totalDur/iterations << " ms" << std::endl;

    return final;
}

int main()
{

    std::cout << "Starting simulation..." << std::endl;
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 2, 1, 10, 293); // 293K = 20C
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
        std::cout << "Inserted " << i << " points\n";
        i++;
    }


    initialiseSimulationValuesFile(initialPoints);
    Octree final = loop(tree, 50, 3, 3);

    std::getchar();
    return 0;
}