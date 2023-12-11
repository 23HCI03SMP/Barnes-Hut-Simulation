#include <iostream>
#include "include/barnesHut.h"
#include <csignal>
#include <fstream>

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
    // Lorentz's force
    // qvx * BX = qvY * BY = qvZ * BZ = F = 0

    float rvx = b->velocityX - node->velocityX; // relative velocity x
    float rvy = b->velocityY - node->velocityY; // relative velocity y
    float rvz = b->velocityZ - node->velocityZ; // relative velocity z

    float crossVelX = 0, crossVelY = 0, crossVelZ = 0;
    float Bx = 0, By = 0, Bz = 0;
    float Ex = 0, Ey = 0, Ez = 0;

    float distance = sqrt(pow(node->point->x - b->point->x, 2) + pow(node->point->y - b->point->y, 2) + pow(node->point->z - b->point->z, 2));

    float posNodeCharge = isExternalNode(node) ? node->charge : node->positiveCharge;
    float negNodeCharge = isExternalNode(node) ? node->charge : node->negativeCharge;
    // float posNodeCharge = isExternalNode(node) ? node->charge > 0 ? node->charge : 0 : node->positiveCharge;
    // float negNodeCharge = isExternalNode(node) ? node->charge < 0 ? node->charge : 0 : node->negativeCharge;

    if (distance != 0)
    { // Coulomb's Law
        if (posdx != 0)
        {
            Ex += K_E * posNodeCharge * posdx / (distance * distance * distance);
            By += K_BS * (posNodeCharge * rvz * posdx) / (abs(distance * distance * distance));
            Bz += K_BS * (posNodeCharge * -rvy * posdx) / (abs(distance * distance * distance));
        }

        if (negdx != 0)
        {
            Ex -= K_E * negNodeCharge * negdx / (distance * distance * distance);
            By -= K_BS * (negNodeCharge * rvz * negdx) / (abs(distance * distance * distance));
            Bz -= K_BS * (negNodeCharge * -rvy * negdx) / (abs(distance * distance * distance));
        }

        if (posdy != 0)
        {
            Bx += K_BS * (posNodeCharge * -rvz * posdy) / (abs(distance * distance * distance));
            Ey += K_E * posNodeCharge * posdy / (distance * distance * distance);
            Bz += K_BS * (posNodeCharge * rvx * posdy) / (abs(distance * distance * distance));
        }

        if (negdy != 0)
        {
            Bx -= K_BS * (negNodeCharge * -rvz * negdy) / (abs(distance * distance * distance));
            Ey -= K_E * negNodeCharge * negdy / (distance * distance * distance);
            Bz -= K_BS * (negNodeCharge * rvx * negdy) / (abs(distance * distance * distance));
        }

        if (posdz != 0)
        {
            Bx += K_BS * (posNodeCharge * rvy * posdz) / (abs(distance * distance * distance));
            By += K_BS * (posNodeCharge * -rvx * posdz) / (abs(distance * distance * distance));
            Ez += K_E * posNodeCharge * posdz / (distance * distance * distance);
        }

        if (negdz != 0)
        {
            Bx -= K_BS * (negNodeCharge * rvy * negdz) / (abs(distance * distance * distance));
            By -= K_BS * (negNodeCharge * -rvx * negdz) / (abs(distance * distance * distance));
            Ez -= K_E * negNodeCharge * negdz / (distance * distance * distance);
        }
    }

    // // output values to forces.txt
    // std::ofstream forcesFile;
    // forcesFile.open("forces.txt", std::ios_base::app);
    // forcesFile << "Ex: " << Ex << " Ey: " << Ey << " Ez: " << Ez << " Bx: " << Bx << " By: " << By << " Bz: " << Bz << std::endl;

    // if (posdx < 0 || negdx < 0)
    // {
    //     Ex = -Ex;
    //     Bx = -Bx;
    // }
    // if (posdy < 0 || negdy < 0)
    // {
    //     Ey = -Ey;
    //     By = -By;
    // }
    // if (posdz < 0 || negdz < 0)
    // {
    //     Ez = -Ez;
    //     Bz = -Bz;
    // }

    // Electric field set to default after every timestep
    b->electricFieldX = b->electricFieldX + Ex;
    b->electricFieldY = b->electricFieldY + Ey;
    b->electricFieldZ = b->electricFieldZ + Ez;

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

    if (node->positiveCoc->x != -1 && b->positiveCoc->x != -1)
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

    // check if node is empty or whether it contains b or cell is b
    if (node->mass == 0 || (!isExternalNode(node) && !cell_contains_position(node, b->point)) || node == b)
    {
        return;
    }

    if (isExternalNode(node))
    {
        float distance = sqrt(pow(node->point->x - b->point->x, 2) + pow(node->point->y - b->point->y, 2) + pow(node->point->z - b->point->z, 2));

        // if (distance == 0)
        // {
        //     // check if node pointer is the same as b pointer
        //     std::cout << (node == b) << std::endl;

        //     asm("int3");
        // }

        // calculate potential energy between node and b
        float pe = (K_E * node->charge * b->charge) / distance;

        // update potential energy
        b->potentialEnergy += pe;
        node->potentialEnergy += pe;

        // if (pe == 0) {
        //     asm("int3");
        // }

        // std::cout << "Potential energy: " << pe << " Charge node: " << node->charge << " Charge b: " << b->charge << " distance: " << distance << std::endl;

        addForce(node, b, posdx, posdy, posdz, negdx, negdy, negdz);
        return;
    }

    // calculate distance between node and b
    float distance = sqrt(pow(posdx, 2) + pow(posdy, 2) + pow(posdz, 2));
    float length = abs(node->minPoints->x - node->maxPoints->x);
    float theta = length / distance;

    // calculate theta (length/distance)
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