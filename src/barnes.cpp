#include <iostream>
#include "include/barnesHut.h"

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool Barnes::isExternalNode(Octree *&octree)
{
    return octree->children.size() == 0;
}

bool cell_contains_position(Octree *&cell, Point *&pos) {
    if (pos->x < cell->minPoints->x || pos->x > cell->maxPoints->x) {
        return false;
    }
    if (pos->y < cell->minPoints->y || pos->y > cell->maxPoints->y) {
        return false;
    }
    if (pos->z < cell->minPoints->z || pos->z > cell->maxPoints->z) {
        return false;
    }
    return true;
}

void addForce(Octree *&node, Octree *&b, float dx, float dy, float dz)
{
        float forceX = 0;
        float forceY = 0;
        float forceZ = 0;

        //Coulomb's Law
        if (dx != 0)
        {
            forceX = K_E * ((node->point->charge * b->point->charge) / (dx * dx));
        }
        if (dy != 0)
        {
            forceY = K_E * ((node->point->charge * b->point->charge) / (dy * dy));
        }
        if (dz != 0)
        {
            forceZ = K_E * ((node->point->charge * b->point->charge) / (dz * dz));
        }

        if (dx < 0)
        {
            forceX = -forceX;
        }
        if (dy < 0)
        {
            forceY = -forceY;
        }
        if (dz < 0)
        {
            forceZ = -forceZ;
        }

        b->forceX += forceX;
        b->forceY += forceY;
        b->forceZ += forceZ;
}

void Barnes::calcForce(Octree *&node, Octree *&b, float thetaLimit)
{
    // if negative, force on b is in the negative direction
    float dx = node->com->x - b->com->x;
    float dy = node->com->y - b->com->y;
    float dz = node->com->z - b->com->z;

    //check if node is empty or whether it contains b
    if (!cell_contains_position(node, b->point) || node->point->mass == 0 || !isExternalNode(node))
    {
        return;
    }

    if (isExternalNode(node))
    {
        addForce(node, b, dx, dy, dz);
        return;
    }

    // calculate theta (length/distance)
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    float theta = length/distance;

    // if theta < 0.5(arbitrary number), treat as a single body
    if (theta < thetaLimit)
    {
        addForce(node, b, dx, dy, dz);
        return;
    }

    for (Octree *child : node->children)
    {
        calcForce(child, b, thetaLimit);
    }
}
