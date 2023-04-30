#include "include/barnesHut.h"
#include <iostream>

Octree loop(Octree *octree, int iterations, float timeStep)
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
                    barnes.calcForce(child, child2, 0.5);
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
    std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 1, 20, 293); // 293K = 20C
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

    initialiseSimulationValuesFile(initialPoints);
    Octree final = loop(tree_ptr, 200, 1e-10);

    std::getchar();
    return 0;
}