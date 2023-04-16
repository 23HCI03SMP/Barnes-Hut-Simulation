#include "include/barnesHut.h"
#include <iostream>

int main()
{
    Octree tree = Octree(1, 1, 1, 5, 5, 5, 1);
    Octree* tree_ptr = &tree;

    tree.insert(tree_ptr, 1, 1, 1, 50);
    tree.insert(tree_ptr, 5, 5, 5, 100);

    // tree.insert(1, 1, 1, 1);
    tree.insert(tree_ptr, 5, 5, 5, 100);

    // std::cout << (tree.find(1, 2, 3)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    //barnes.cpp

    Barnes barnes;
    barnes.calcForce(tree_ptr->children[0], tree_ptr->children[7], 0.5);

    std::getchar();
    return 0;
}