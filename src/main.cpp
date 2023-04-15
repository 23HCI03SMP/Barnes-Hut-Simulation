#include "include/barnesHut.h"
#include <iostream>

int main()
{
    Octree tree = Octree(1, 1, 1, 5, 5, 5, 0);
    Octree* tree_ptr = &tree;

    tree.insert(tree_ptr, 2, 2, 2, 100);
    tree.insert(tree_ptr, 4, 4, 4, 100);

    // tree.insert(1, 1, 1, 1);
    // tree.insert(tree_ptr, 5, 5, 5, 100);

    // std::cout << (tree.find(1, 2, 3)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");
    std::getchar();
    return 0;
}