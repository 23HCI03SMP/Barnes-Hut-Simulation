#include "include/barnesHut.h"
#include <iostream>

std::vector<Octree *> getChildren(Octree *&volume)
{
    std::vector<Octree *> childrenList;
    for (Octree *child : volume->children)
    {
        if (child->children.size() == 0 && child->point != nullptr && child->point->x != -1)
        {
            childrenList.push_back(child);
        }
        std::vector<Octree *> recursChildren = getChildren(child);
        childrenList.insert(childrenList.end(), recursChildren.begin(), recursChildren.end());
    }
    return childrenList;
}

Octree *loop(int iteration, Octree *octree)
{
    Octree *finalPtr;

    while (iteration != 0)
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
        Octree final = sim.mainLoop(octree, 1, 1e-5);
        finalPtr = &final;

        iteration--;
    }
    return finalPtr;
}

int main()
{
    std::vector<std::array<float, 7>> points = generatePoints(1, 1, 1, 5, 5, 5, 1, 100, 293); // 293K = 20C
    generateFile(points);


    std::vector<std::array<float, 7>> a = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    // tree.insert(tree_ptr, 1, 1, 1, 1, 1);
    // tree.insert(tree_ptr, 5, 5, 5, 1, 1);
    // tree.insert(tree_ptr, 2.79788, 2.06269, 4.75517, 1, 1);
    // tree.insert(tree_ptr, 3.27199, 2.08053, 4.75517, 1, 1);
    // tree.insert(tree_ptr, 3.67951, 2.32349, 4.75517, 1, 1);

    for (std::array<float, 7> point: a)
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

    // tree.insert(1, 1, 1, 1);
    // tree.insert(tree_ptr, 5, 5, 5, 100);

    // std::cout << (tree.find(2, 1, 1)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(1, 2, 3)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // barnes.cpp
    Octree *final = loop(1, tree_ptr);
    std::getchar();
    return 0;
}