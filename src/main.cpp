#include "include/barnesHut.h"
#include <iostream> 

int main()
{
    // auto points = generatePoints(1, 1, 1, 5, 5, 5, 1, 0.5);
    // generateFile(points);

    // std::vector<std::array<float, 4>> a = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    tree.insert(tree_ptr, 3.84147, 3.4597, 4.75517, 1, 1);
    tree.insert(tree_ptr, 2.10208, 2.66365, 4.75517, 1, 1);
    // tree.insert(tree_ptr, 2.37325, 2.27434, 4.75517, 1, 1);
    // tree.insert(tree_ptr, 2.79788, 2.06269, 4.75517, 1, 1);
    tree.insert(tree_ptr, 3.27199, 2.08053, 4.75517, 1, 1);
    tree.insert(tree_ptr, 3.67951, 2.32349, 4.75517, 1, 1);

    // for (std::array<float, 4> point: a)
    // {
    //     tree.insert(tree_ptr, point[0], point[1], point[2], point[3], point[3]);
    // }

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

    Barnes barnes;
    barnes.calcForce(tree_ptr->children[0], tree_ptr->children[7], 0.5);

    Simulation sim = Simulation();
    Octree final = sim.mainLoop(tree_ptr, 1, 0.5);

    std::getchar();
    return 0;
}