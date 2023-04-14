#include "include/barnesHut.h"
#include <iostream>

int main()
{
    Octree tree = Octree(1, 1, 1, 5, 5, 5, 0);
    // tree.insert(2, 2, 2, 1);
    // tree.insert(4, 4, 4, 1);

    tree.insert(1, 2, 3, 1);
    tree.insert(2, 3, 1, 1);
    tree.insert(5, 5, 1, 1);

    // std::cout << (tree.find(1, 2, 3)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");

    tree.insert(3, 4, 4, 5);

    // std::cout << (tree.find(3, 4, 4)
    //                   ? "Found\n"
    //                   : "Not Found\n");
    std::getchar();
    return 0;
}