#include "include/octree.h"
#include <iostream>

int main() {
    double a = pi();
    std::cout << a << std::endl;

    Particle particle = {0, 0, 0};
    Bbox box;
    Node node;
    node.insertParticle(particle, box);
    for (int i = 0; i < (signed)(box.children).size(); i++){
        std::cout << box.children[i].particle.position[0];
        std::cout << box.children[i].particle.position[1];
        std::cout << box.children[i].particle.position[1] << std::endl;
    }

    
    return 0;
}