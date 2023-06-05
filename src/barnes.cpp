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

    float posNodeCharge = isExternalNode(node) ? node->charge : node->positiveCharge;
    float negNodeCharge = isExternalNode(node) ? node->charge : node->negativeCharge;

    // Coulomb's Law
    if (posdx != 0)
    {
        forceX += K_E * ((posNodeCharge * b->charge) / (posdx * posdx));
        forceY += K_BS * (posNodeCharge * b->charge * crossVelZ * posdx) / (abs(posdx * posdx * posdx));
        forceZ += K_BS * (posNodeCharge * b->charge * -crossVelY * posdx) / (abs(posdx * posdx * posdx));
    }

    if (negdx != 0)
    {
        forceX -= K_E * ((negNodeCharge * b->charge) / (negdx * negdx));
        forceY -= K_BS * (negNodeCharge * b->charge * crossVelZ * negdx) / (abs(negdx * negdx * negdx));
        forceZ -= K_BS * (negNodeCharge * b->charge * -crossVelY * negdx) / (abs(negdx * negdx * negdx));
    }

    if (posdy != 0)
    {
        forceX += K_BS * (posNodeCharge * b->charge * crossVelZ * posdy) / (abs(posdy * posdy * posdy));
        forceY += K_E * ((posNodeCharge * b->charge) / (posdy * posdy));
        forceZ += K_BS * (posNodeCharge * b->charge * crossVelX * posdy) / (abs(posdy * posdy * posdy));
    }

    if (negdy != 0)
    {
        forceX -= K_BS * (negNodeCharge * b->charge * crossVelZ * negdy) / (abs(negdy * negdy * negdy));
        forceY -= K_E * ((negNodeCharge * b->charge) / (negdy * negdy));
        forceZ -= K_BS * (negNodeCharge * b->charge * crossVelX * negdy) / (abs(negdy * negdy * negdy));
    }

    if (posdz != 0)
    {
        forceX += K_BS * (posNodeCharge * b->charge * -crossVelY * posdz) / (abs(posdz * posdz * posdz));
        forceY += K_BS * (posNodeCharge * b->charge * -crossVelX * posdz) / (abs(posdz * posdz * posdz));
        forceZ += K_E * ((posNodeCharge * b->charge) / (posdz * posdz));
    }

    if (negdz != 0)
    {
        forceX -= K_BS * (negNodeCharge * b->negativeCharge * -crossVelY * negdz) / (abs(negdz * negdz * negdz));
        forceY -= K_BS * (negNodeCharge * b->negativeCharge * -crossVelX * negdz) / (abs(negdz * negdz * negdz));
        forceZ -= K_E * ((negNodeCharge * b->negativeCharge) / (negdz * negdz));
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

    //External Magnetic Field
    b->forceX = b->charge*(b->velocityZ * b->magneticFieldY);
    b->forceX = b->forceX + b->charge*(-b->velocityY * b->magneticFieldZ);
    b->forceY = b->charge*(b->velocityX * b->magneticFieldZ);
    b->forceY = b->forceY + b->charge*(-b->velocityZ * b->magneticFieldX);
    b->forceZ = b->charge*(b->velocityY * b->magneticFieldX);
    b->forceZ = b->forceZ + b->charge*(-b->velocityX * b->magneticFieldY);

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