#include <iostream>
#include "include/barnesHut.h"

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool Barnes::isExternalNode(Octree *octree)
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

        //Lorentz's force
        //qvx * BX = qvY * BY = qvZ * BZ = F = 0

        float vx1 = b->velocityX;
        float vx2 = node->velocityX;
        float vy1 = b->velocityY;
        float vy2 = node->velocityY;
        float vz1 = b->velocityZ;
        float vz2 = node->velocityZ;

        float crossVelX = 0;
        float crossVelY = 0;
        float crossVelZ = 0;

        crossVelX = vz1 * vy2;
        crossVelX += -vy1 * vz2;

        crossVelY += vz1 * vx2;
        crossVelY += -vx1 * vz2;

        crossVelZ += vx1 * vy2;
        crossVelZ += -vy1 * vx2;

        //Coulomb's Law
        if (dx != 0)
        {
            forceX += K_E * ((node->charge * b->charge) / (dx * dx));
            forceY += K_BS * (node->charge * b->charge * crossVelZ * dx)/(abs(dx*dx*dx));
            forceZ += K_BS * (node->charge * b->charge * -crossVelY * dx)/(abs(dx*dx*dx));
        }
        if (dy != 0)
        {
            forceX += K_BS * (node->charge * b->charge * crossVelZ * dy)/(abs(dy*dy*dy));
            forceY += K_E * ((node->charge * b->charge) / (dy * dy));
            forceZ += K_BS * (node->charge * b->charge * crossVelX * dy)/(abs(dy*dy*dy));
        }
        if (dz != 0)
        {
            forceX += K_BS * (node->charge * b->charge * -crossVelY * dz)/(abs(dz*dz*dz));
            forceY += K_BS * (node->charge * b->charge * -crossVelX * dz)/(abs(dz*dz*dz));
            forceZ += K_E * ((node->charge * b->charge) / (dz * dz));
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

        b->forceX = forceX;
        b->forceY = forceY;
        b->forceZ = forceZ;

        // if (((forceX == 0 || forceY == 0 || forceZ == 0) && node != b) || forceX < 1e-8 || forceY < 1e-8 || forceZ < 1e-8)
        // {
        //     __asm__("int $3");
        // }
}

void Barnes::calcForce(Octree *node, Octree *b, float thetaLimit)
{
    // if negative, force on b is in the negative direction
    float dx = node->coc->x - b->coc->x;
    float dy = node->coc->y - b->coc->y;
    float dz = node->coc->z - b->coc->z;

    //check if node is empty or whether it contains b
    if (node->mass == 0 || (!isExternalNode(node) && !cell_contains_position(node, b->point)))
    {
        //std::cout << "bad" << std::endl;
        return;
    }

    if (isExternalNode(node))
    {
        addForce(node, b, dx, dy, dz);
        //std::cout << "external" << std::endl;
        return;
    }

    // calculate theta (length/distance)
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    float theta = length/distance;

    //std::cout << "theta: " << theta << std::endl;

    // if theta < 0.5(arbitrary number), treat as a single body
    if (theta < thetaLimit)
    {
        //std::cout << "less\n";
        addForce(node, b, dx, dy, dz);
        return;
    }

    //std::cout << "more\n";

    for (Octree *child : node->children)
    {
        calcForce(child, b, thetaLimit);
    }
}
