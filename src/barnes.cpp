#include <iostream>
#include "include/barnesHut.h"

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool Barnes::isExternalNode(Octree *octree)
{
    return octree->children.size() == 0;
}

bool cell_contains_position(Octree *cell, Point *pos)
{
    if (pos->x < cell->minPoints->x || pos->x > cell->maxPoints->x)
    {
        return false;
    }
    if (pos->y < cell->minPoints->y || pos->y > cell->maxPoints->y)
    {
        return false;
    }
    if (pos->z < cell->minPoints->z || pos->z > cell->maxPoints->z)
    {
        return false;
    }
    return true;
}

void Barnes::addForce(Octree *node, Octree *b, float posdx, float posdy, float posdz, float negdx, float negdy, float negdz)
{
    float forceX = 0;
    float forceY = 0;
    float forceZ = 0;

    // Lorentz's force
    // qvx * BX = qvY * BY = qvZ * BZ = F = 0

    float rvx = b->velocityX - node->velocityX; //relative velocity x
    float rvy = b->velocityY - node->velocityY; //relative velocity y
    float rvz = b->velocityZ - node->velocityZ; //relative velocity z

    float crossVelX = 0;
    float crossVelY = 0;
    float crossVelZ = 0;

    float Bx = 0;
    float By = 0;
    float Bz = 0;

    float posNodeCharge = isExternalNode(node) ? node->charge : node->positiveCharge;
    float negNodeCharge = isExternalNode(node) ? node->charge : node->negativeCharge;

    // Coulomb's Law
    if (posdx != 0)
    {
        forceX += K_E * ((posNodeCharge * b->charge) / (posdx * posdx));
        By += K_BS*(posNodeCharge * rvz * posdx) / (abs(posdx * posdx * posdx));
        Bz += K_BS*(posNodeCharge * -rvy * posdx) / (abs(posdx * posdx * posdx));

        
    }

    if (negdx != 0)
    {
        forceX -= K_E * ((negNodeCharge * b->charge) / (negdx * negdx));
        By -= K_BS*(negNodeCharge * rvz * negdx) / (abs(negdx * negdx * negdx));
        Bz -= K_BS*(negNodeCharge * -rvy * negdx) / (abs(negdx * negdx * negdx));
    }

    if (posdy != 0)
    {
        Bx += K_BS*(posNodeCharge * -rvz * posdy) / (abs(posdy * posdy * posdy));
        forceY += K_E * ((posNodeCharge * b->charge) / (posdy * posdy));
        Bz += K_BS*(posNodeCharge * rvx * posdy) / (abs(posdy * posdy * posdy));
    }

    if (negdy != 0)
    {
        Bx -= K_BS*(negNodeCharge * -rvz * negdy) / (abs(negdy * negdy * negdy));
        forceY -= K_E * ((negNodeCharge * b->charge) / (negdy * negdy));
        Bz -= K_BS*(negNodeCharge * rvx * negdy) / (abs(negdy * negdy * negdy));
    }

    if (posdz != 0)
    {
        Bx += K_BS*(posNodeCharge * rvy * posdz) / (abs(posdz * posdz * posdz));
        By += K_BS*(posNodeCharge * -rvx * posdz) / (abs(posdz * posdz * posdz));
        forceZ += K_E * ((posNodeCharge * b->charge) / (posdz * posdz));
    }

    if (negdz != 0)
    {
        Bx -= K_BS*(negNodeCharge * rvy * negdz) / (abs(negdz * negdz * negdz));
        By -= K_BS*(negNodeCharge * -rvx * negdz) / (abs(negdz * negdz * negdz));
        forceZ -= K_E * ((negNodeCharge * b->negativeCharge) / (negdz * negdz));
    }

    if (posdx < 0)
    {
        forceX = -forceX;
        Bx = -Bx;
    }
    if (posdy < 0)
    {
        forceY = -forceY;
        By = -By;
    }
    if (posdz < 0)
    {
        forceZ = -forceZ;
        Bz = -Bz;
    }

    //need a negdx dy dz?

    b->forceX = b->forceX + forceX;
    b->forceY = b->forceY + forceY;
    b->forceZ = b->forceZ + forceZ;

    b->magneticFieldX = b->magneticFieldX + Bx;
    b->magneticFieldY = b->magneticFieldY + By;
    b->magneticFieldZ = b->magneticFieldZ + Bz;
}

void Barnes::calcForce(Octree *node, Octree *b, float thetaLimit)
{
    // if negative, force on b is in the negative direction
    float posdx = 0;
    float posdy = 0;
    float posdz = 0;

    float negdx = 0;
    float negdy = 0;
    float negdz = 0;

    if (node->positiveCoc->x != -1 || b->positiveCoc->x != -1)
    {
        posdx = node->positiveCoc->x - b->positiveCoc->x;
        posdy = node->positiveCoc->y - b->positiveCoc->y;
        posdz = node->positiveCoc->z - b->positiveCoc->z;
    }

    if (node->negativeCoc->x != -1 && b->negativeCoc->x != -1)
    {
        negdx = node->negativeCoc->x - b->negativeCoc->x;
        negdy = node->negativeCoc->y - b->negativeCoc->y;
        negdz = node->negativeCoc->z - b->negativeCoc->z;
    }

    // check if node is empty or whether it contains b
    if (node->mass == 0 || (!isExternalNode(node) && !cell_contains_position(node, b->point)))
    {
        // std::cout << "bad" << std::endl;
        return;
    }

    if (isExternalNode(node))
    {
        addForce(node, b, posdx, posdy, posdz, negdx, negdy, negdz);
        return;
    }

    // calculate theta (length/distance)
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(posdx, 2) + pow(posdy, 2) + pow(posdz, 2));
    float theta = length / distance;

    // if theta < 0.5(arbitrary number), treat as a single body
    if (theta < thetaLimit)
    {
        addForce(node, b, posdx, posdy, posdz, negdx, negdy, negdz);
        return;
    }

    for (Octree *child : node->children)
    {
        calcForce(child, b, thetaLimit);
    }
}