#include <iostream>
#include "include/barnesHut.h"

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool Barnes::isExternalNode(Octree *&octree) {
    return octree->children.size() == 0;
}

void Barnes::calcForce(Octree *&node, Octree *&b, float thetaLimit) {
    // if negative, force on b is in the negative direction
    float dx = node->com->x - b->com->x;
    float dy = node->com->y - b->com->y;
    float dz = node->com->z - b->com->z;
    
    if (isExternalNode(node)) {
        // Coulomb's law
        // std::cout << "External" << std::endl;
        float forceX = 0;
        float forceY = 0;
        float forceZ = 0;
        if (dx != 0)
        {
            forceX = K_E*((node->charge * b->charge)/(dx * dx));
        }
        if (dy != 0)
        {
            forceY = K_E*((node->charge * b->charge)/(dy * dy));
        }
        if (dz != 0)
        {
            forceZ = K_E*((node->charge * b->charge)/(dz * dz));
        }

        if (dx < 0) {
            forceX = -forceX;
        }
        if (dy < 0) {
            forceY = -forceY;
        }
        if (dz < 0) {
            forceZ = -forceZ;
        }

        b->forceX += forceX;
        b->forceY += forceY;
        b->forceZ += forceZ;

        return;
    }
    //calculate theta (length/distance)
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    float theta = length/distance;

    //if theta < 0.5(arbitrary number), treat as a single body
    if (theta < thetaLimit) {
        // std::cout << "Theta check" << std::endl;
        float forceX = K_E*((node->charge * b->charge)/(dx * dx));
        float forceY = K_E*((node->charge * b->charge)/(dy * dy));
        float forceZ = K_E*((node->charge * b->charge)/(dz * dz));

        if (dx < 0) {
            forceX = -forceX;
        }
        if (dy < 0) {
            forceY = -forceY;
        }
        if (dz < 0) {
            forceZ = -forceZ;
        }

        b->forceX += forceX;
        b->forceY += forceY;
        b->forceZ += forceZ;
        return;
    }

    //else, run recursively on children
    for(Octree* child : node->children) {
        // std::cout << "child recursion" << std::endl;
        float dx = child->com->x - b->com->x;
        float dy = child->com->y - b->com->y;
        float dz = child->com->z - b->com->z;

        float forceX = K_E*((child->charge * b->charge)/(dx * dx));
        float forceY = K_E*((child->charge * b->charge)/(dy * dy));
        float forceZ = K_E*((child->charge * b->charge)/(dz * dz));

        if (dx < 0) {
            forceX = -forceX;
        }
        if (dy < 0) {
            forceY = -forceY;
        }
        if (dz < 0) {
            forceZ = -forceZ;
        }

        b->forceX += forceX;
        b->forceY += forceY;
        b->forceZ += forceZ;
    }
}
