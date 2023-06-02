#include <iostream>
#include "include/barnesHut.h"

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool Barnes::isExternalNode(Octree *octree)
{
    return octree->children.size() == 0;
}

bool cell_contains_position(Octree *cell, Point *pos) {
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

void addForce(Octree *node, Octree *b, float posdx, float posdy, float posdz, float negdx, float negdy, float negdz)
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
        if (posdx != 0 && negdx != 0)
        {
            forceX += K_E * ((node->positiveCharge * b->positiveCharge) / (posdx * posdx));
            forceY += K_BS * (node->positiveCharge * b->positiveCharge * crossVelZ * posdx)/(abs(posdx*posdx*posdx));
            forceZ += K_BS * (node->positiveCharge * b->positiveCharge * -crossVelY * posdx)/(abs(posdx*posdx*posdx));

            forceX -= K_E * ((node->negativeCharge * b->negativeCharge) / (negdx * negdx));
            forceY -= K_BS * (node->negativeCharge * b->negativeCharge * crossVelZ * negdx)/(abs(negdx*negdx*negdx));
            forceZ -= K_BS * (node->negativeCharge * b->negativeCharge * -crossVelY * negdx)/(abs(negdx*negdx*negdx));
        }
        if (posdy != 0 && negdy != 0)
        {
            forceX += K_BS * (node->positiveCharge * b->positiveCharge * crossVelZ * posdy)/(abs(posdy*posdy*posdy));
            forceY += K_E * ((node->positiveCharge * b->positiveCharge) / (posdy * posdy));
            forceZ += K_BS * (node->positiveCharge * b->positiveCharge * crossVelX * posdy)/(abs(posdy*posdy*posdy));

            forceX -= K_BS * (node->negativeCharge * b->negativeCharge * crossVelZ * negdy)/(abs(negdy*negdy*negdy));
            forceY -= K_E * ((node->negativeCharge * b->negativeCharge) / (negdy * negdy));
            forceZ -= K_BS * (node->negativeCharge * b->negativeCharge * crossVelX * negdy)/(abs(negdy*negdy*negdy));
        }
        if (posdz != 0 && negdz != 0)
        {
            forceX += K_BS * (node->positiveCharge * b->positiveCharge * -crossVelY * posdz)/(abs(posdz*posdz*posdz));
            forceY += K_BS * (node->positiveCharge * b->positiveCharge * -crossVelX * posdz)/(abs(posdz*posdz*posdz));
            forceZ += K_E * ((node->positiveCharge * b->positiveCharge) / (posdz * posdz));
            
            forceX -= K_BS * (node->negativeCharge * b->negativeCharge * -crossVelY * negdz)/(abs(negdz*negdz*negdz));
            forceY -= K_BS * (node->negativeCharge * b->negativeCharge * -crossVelX * negdz)/(abs(negdz*negdz*negdz));
            forceZ -= K_E * ((node->negativeCharge * b->negativeCharge) / (negdz * negdz));
        }

        if (posdx < 0)
        {
            forceX = -forceX;
        }
        if (posdy < 0)
        {
            forceY = -forceY;
        }
        if (posdz < 0)
        {
            forceZ = -forceZ;
        }

        b->forceX = forceX;
        b->forceY = forceY;
        b->forceZ = forceZ;

        // if (((forceX == 0 || forceY == 0 || forceZ == 0) && node != b))
        // {
        //     __asm__("int $3");
        // }
}

void Barnes::calcForce(Octree *node, Octree *b, float thetaLimit)
{
    // if negative, force on b is in the negative direction
    float posdx = node->positiveCoc->x - b->positiveCoc->x;
    float posdy = node->positiveCoc->y - b->positiveCoc->y;
    float posdz = node->positiveCoc->z - b->positiveCoc->z;

    float negdx = 0;
    float negdy = 0;
    float negdz = 0;

    //This is wrong, but I don't know how to fix it b->negativeCoc != nullptr
    if (b->negativeCoc == nullptr)
    {
        negdx = node->negativeCoc->x - b->negativeCoc->x;
        negdy = node->negativeCoc->y - b->negativeCoc->y;
        negdz = node->negativeCoc->z - b->negativeCoc->z;
    }

    //check if node is empty or whether it contains b
    if (node->mass == 0 || (!isExternalNode(node) && !cell_contains_position(node, b->point)))
    {
        //std::cout << "bad" << std::endl;
        return;
    }

    if (isExternalNode(node))
    {
        addForce(node, b, posdx, posdy, posdz, negdx, negdy, negdz);
        //std::cout << "external" << std::endl;
        return;
    }

    // calculate theta (length/distance)
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(posdx, 2) + pow(posdy, 2) + pow(posdz, 2));
    float theta = length/distance;

    //std::cout << "theta: " << theta << std::endl;

    // if theta < 0.5(arbitrary number), treat as a single body
    if (theta < thetaLimit)
    {
        //std::cout << "less\n";
        addForce(node, b, posdx, posdy, posdz, negdx, negdy, negdz);
        return;
    }

    //std::cout << "more\n";

    for (Octree *child : node->children)
    {
        calcForce(child, b, thetaLimit);
    }
}