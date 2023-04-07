#include "include/octree.h"
#include <iostream>

int main() {
    Particle particle = {0, 0, 0};
    Bbox box;
    Node node;
    node.insertParticle(particle, box);
    for (int i = 0; i < size(box.children); i++){
        // Particle output = box.children[i].particle.position;
    }

    return 0;
}