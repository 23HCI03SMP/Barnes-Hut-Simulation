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

void Barnes::addForce(Octree *node, Octree *b, double posdx, double posdy, double posdz, double negdx, double negdy, double negdz)
{
    // Lorentz's force
    // qvx * BX = qvY * BY = qvZ * BZ = F = 0

    double rvx = b->velocityX - node->velocityX; // relative velocity x
    double rvy = b->velocityY - node->velocityY; // relative velocity y
    double rvz = b->velocityZ - node->velocityZ; // relative velocity z

    double crossVelX = 0, crossVelY = 0, crossVelZ = 0;
    double Bx = 0, By = 0, Bz = 0;
    double Ex = 0, Ey = 0, Ez = 0;

    double posDistance = sqrt(pow(posdx, 2) + pow(posdy, 2) + pow(posdz, 2));
    double negDistance = sqrt(pow(negdx, 2) + pow(negdy, 2) + pow(negdz, 2));

    double posNodeCharge = isExternalNode(node) ? node->charge : node->positiveCharge;
    double negNodeCharge = isExternalNode(node) ? node->charge : node->negativeCharge;
    // double posNodeCharge = isExternalNode(node) ? node->charge > 0 ? node->charge : 0 : node->positiveCharge;
    // double negNodeCharge = isExternalNode(node) ? node->charge < 0 ? node->charge : 0 : node->negativeCharge;

    if (posDistance != 0)
    { // Coulomb's Law
        if (posdx != 0)
        {
            Ex += K_E * posNodeCharge * posdx / (posDistance * posDistance * posDistance);
            By += K_BS * (posNodeCharge * rvz * posdx) / (abs(posDistance * posDistance * posDistance));
            Bz += K_BS * (posNodeCharge * -rvy * posdx) / (abs(posDistance * posDistance * posDistance));
        }

        if (posdy != 0)
        {
            Bx += K_BS * (posNodeCharge * -rvz * posdy) / (abs(posDistance * posDistance * posDistance));
            Ey += K_E * posNodeCharge * posdy / (posDistance * posDistance * posDistance);
            Bz += K_BS * (posNodeCharge * rvx * posdy) / (abs(posDistance * posDistance * posDistance));
        }

        if (posdz != 0)
        {
            Bx += K_BS * (posNodeCharge * rvy * posdz) / (abs(posDistance * posDistance * posDistance));
            By += K_BS * (posNodeCharge * -rvx * posdz) / (abs(posDistance * posDistance * posDistance));
            Ez += K_E * posNodeCharge * posdz / (posDistance * posDistance * posDistance);
        }
    }

    if (negDistance != 0)
    {
        if (negdx != 0)
        {
            Ex -= K_E * negNodeCharge * negdx / (negNodeCharge * negNodeCharge * negNodeCharge);
            By -= K_BS * (negNodeCharge * rvz * negdx) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
            Bz -= K_BS * (negNodeCharge * -rvy * negdx) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
        }
        if (negdy != 0)
        {
            Bx -= K_BS * (negNodeCharge * -rvz * negdy) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
            Ey -= K_E * negNodeCharge * negdy / (negNodeCharge * negNodeCharge * negNodeCharge);
            Bz -= K_BS * (negNodeCharge * rvx * negdy) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
        }
        if (negdz != 0)
        {
            Bx -= K_BS * (negNodeCharge * rvy * negdz) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
            By -= K_BS * (negNodeCharge * -rvx * negdz) / (abs(negNodeCharge * negNodeCharge * negNodeCharge));
            Ez -= K_E * negNodeCharge * negdz / (negNodeCharge * negNodeCharge * negNodeCharge);
        }
    }

    // // output values to forces.csv
    // std::ofstream forcesFile;
    // forcesFile.open("forces.csv", std::ios_base::app);
    // forcesFile << Ex << "," << Ey << "," << Ez << "," << Bx << "," << By << "," << Bz << "," << std::endl;

    if (posdx < 0 || negdx < 0)
    {
        Ex = -Ex;
        Bx = -Bx;
    }
    if (posdy < 0 || negdy < 0)
    {
        Ey = -Ey;
        By = -By;
    }
    if (posdz < 0 || negdz < 0)
    {
        Ez = -Ez;
        Bz = -Bz;
    }

    // Electric field set to default after every timestep
    b->electricFieldX = b->electricFieldX + Ex;
    b->electricFieldY = b->electricFieldY + Ey;
    b->electricFieldZ = b->electricFieldZ + Ez;

    b->magneticFieldX = b->magneticFieldX + Bx;
    b->magneticFieldY = b->magneticFieldY + By;
    b->magneticFieldZ = b->magneticFieldZ + Bz;
}

void Barnes::calcForce(Octree *node, Octree *b, double thetaLimit)
{
    // if negative, force on b is in the negative direction
    double posdx = 0;
    double posdy = 0;
    double posdz = 0;

    double negdx = 0;
    double negdy = 0;
    double negdz = 0;

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
    if ((node->positiveCharge == 0 && node->negativeCharge == 0 && node->charge == 0) || node == b)
    {
        return;
    }

    if (isExternalNode(node))
    {
        double distance = sqrt(pow(node->point->x - b->point->x, 2) + pow(node->point->y - b->point->y, 2) + pow(node->point->z - b->point->z, 2));

        // if (distance == 0)
        // {
        //     // check if node pointer is the same as b pointer
        //     std::cout << (node == b) << std::endl;

        //     asm("int3");
        // }

        // calculate potential energy between node and b
        double pe = (K_E * node->charge * b->charge) / distance;

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

    // calculate distance between node and b node may not have
    // average point of positive and negative COCs of node
    double avgPointx = (node->positiveCoc->x + node->negativeCoc->x) / 2;
    double avgPointy = (node->positiveCoc->y + node->negativeCoc->y) / 2;
    double avgPointz = (node->positiveCoc->z + node->negativeCoc->z) / 2;
    double distance = sqrt(pow(avgPointx - b->point->x, 2) + pow(avgPointy - b->point->y, 2) + pow(avgPointz - b->point->z, 2));

    double length = abs(node->minPoints->x - node->maxPoints->x);
    double theta = length / distance;

    // //output theta to theta.csv
    // std::ofstream thetaFile;
    // thetaFile.open("theta.csv", std::ios_base::app);
    // thetaFile << theta << "," << std::endl;

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