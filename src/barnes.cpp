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
    double dx = node->com->x - b->com->x;
    double dy = node->com->y - b->com->y;
    double dz = node->com->z - b->com->z;
    
    if (isExternalNode(node)) {
        // Coulomb's law
        double forceX = K*((node->mass * b->mass)/(dx * dx));
        double forceY = K*((node->mass * b->mass)/(dy * dy));
        double forceZ = K*((node->mass * b->mass)/(dz * dz));

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
        double forceX = K*((node->mass * b->mass)/(dx * dx));
        double forceY = K*((node->mass * b->mass)/(dy * dy));
        double forceZ = K*((node->mass * b->mass)/(dz * dz));

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
        double dx = child->com->x - b->com->x;
        double dy = child->com->y - b->com->y;
        double dz = child->com->z - b->com->z;

        double forceX = K*((child->mass * b->mass)/(dx * dx));
        double forceY = K*((child->mass * b->mass)/(dy * dy));
        double forceZ = K*((child->mass * b->mass)/(dz * dz));

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
