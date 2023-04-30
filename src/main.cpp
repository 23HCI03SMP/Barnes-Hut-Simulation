#include "include/barnesHut.h"
#include <iostream>

Octree loop(Octree *octree, int iterations, float timeStep)
{
    Octree final = Octree(0, 0, 0, 0, 0, 0);
    Octree *finalPtr;

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
    }

    return final;
}

int main()
{
    std::vector<std::array<float, 7>> points = generatePoints(1, 1, 1, 5, 5, 5, 1, 20, 293); // 293K = 20C
    generateFile(points);

    std::vector<std::array<float, 7>> a = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    for (std::array<float, 7> point : a)
    {
        float x = point[0];
        float y = point[1];
        float z = point[2];
        float vx = point[3];
        float vy = point[4];
        float vz = point[5];
        float mass = point[6];

        tree.insert(tree_ptr, x, y, z, vx, vy, vz, mass, mass);
    }

    Octree final = loop(tree_ptr, 1, 1e-10);


    std::getchar();
    return 0;
}