#include "include/barnesHut.h"
#include <iostream>

int main()
{
    std::vector<std::array<float, 4>> a = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    for (std::array<float, 4> point: a)
    {
        tree.insert(tree_ptr, point[0], point[1], point[2], point[3]);
    }

    // tree.insert(1, 1, 1, 1);
    // tree.insert(tree_ptr, 5, 5, 5, 100);

    std::cout << (tree.find(2, 1, 1)
                      ? "Found\n"
                      : "Not Found\n");

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

    std::getchar();
    return 0;
}