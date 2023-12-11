#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>
#include "include/barnesHut.h"
#include <fstream>

// o represents octet
#define o1 0
#define o2 1
#define o3 2
#define o4 3
#define o5 4
#define o6 5
#define o7 6
#define o8 7

Octree::Octree()
{
    point = new Point();
}

Octree::Octree(std::string alias, double x, double y, double z, double vx, double vy, double vz, double mass, double charge)
{
    point = new Point(x, y, z);

    this->alias = alias;
    this->charge = charge;
    this->mass = mass;
    velocityX = vx;
    velocityY = vy;
    velocityZ = vz;
}

// Initializing a base octree
Octree::Octree(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
    if (maxX < minX || maxY < minY || maxZ < minZ)
    {
        std::cout << "Invalid Inputs";
        return;
    }
    point = nullptr;
    minPoints = new Point(minX, minY, minZ);
    maxPoints = new Point(maxX, maxY, maxZ);

    children.assign(8, nullptr);
    for (int i = o1; i <= o8; i++)
    {
        children[i] = new Octree();
    }
}

bool Octree::find(double x, double y, double z)
{
    // If point is out of bound
    if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z)
        return false;

    // Otherwise perform binary search
    // for each ordinate
    double midx = (minPoints->x + maxPoints->x) / 2;
    double midy = (minPoints->y + maxPoints->y) / 2;
    double midz = (minPoints->z + maxPoints->z) / 2;
    double pos = -1;

    // Deciding the position
    // where to move
    if (x <= midx)
    {
        if (y <= midy)
        {
            if (z <= midz)
                pos = o1;
            else
                pos = o5;
        }
        else
        {
            if (z <= midz)
                pos = o3;
            else
                pos = o7;
        }
    }
    else
    {
        if (y <= midy)
        {
            if (z <= midz)
                pos = o2;
            else
                pos = o6;
        }
        else
        {
            if (z <= midz)
                pos = o4;
            else
                pos = o8;
        }
    }

    // If an internal node is encountered
    if (children[pos]->point == nullptr)
    {
        return children[pos]->find(x, y, z);
    }

    // If an empty node is encountered
    if (children[pos]->point->x == -1)
    {
        return false;
    }

    // If node is found with
    // the given value
    if (x == children[pos]->point->x && y == children[pos]->point->y && z == children[pos]->point->z)
        return true;

    return false;
}

void Octree::insert(Octree *root, Octree *octree)
{
    if (find(octree->point->x, octree->point->y, octree->point->z))
    {
        std::cout << "point already exists" << std::endl;
        return;
    }

    if (octree->point->x < minPoints->x || octree->point->x > maxPoints->x || octree->point->y < minPoints->y || octree->point->y > maxPoints->y || octree->point->z < minPoints->z || octree->point->z > maxPoints->z)
    {
        // std::cout << "Out of bound" << std::endl;
        return;
    }

    double midX = (minPoints->x + maxPoints->x) / 2;
    double midY = (minPoints->y + maxPoints->y) / 2;
    double midZ = (minPoints->z + maxPoints->z) / 2;
    double pos = -1;

    if (octree->point->x <= midX)
    {
        if (octree->point->y <= midY)
        {
            if (octree->point->z <= midZ)
            {
                pos = o1;
            }
            else
            {
                pos = o5;
            }
        }
        else
        {
            if (octree->point->z <= midZ)
            {
                pos = o3;
            }
            else
            {
                pos = o7;
            }
        }
    }
    else
    {
        if (octree->point->y <= midY)
        {
            if (octree->point->z <= midZ)
            {
                pos = o2;
            }
            else
            {
                pos = o6;
            }
        }
        else
        {
            if (octree->point->z <= midZ)
            {
                pos = o4;
            }
            else
            {
                pos = o8;
            }
        }
    }

    if (children[pos]->point == nullptr)
    {
        children[pos]->insert(root, new Octree(octree->alias, octree->point->x, octree->point->y, octree->point->z, octree->velocityX, octree->velocityY, octree->velocityZ, octree->mass, octree->charge));
    }
    else if (children[pos]->point->x == -1)
    {
        delete children[pos];
        children[pos] = octree;
    }
    else
    {
        double x_ = children[pos]->point->x;
        double y_ = children[pos]->point->y;
        double z_ = children[pos]->point->z;
        double vx_ = children[pos]->velocityX;
        double vy_ = children[pos]->velocityY;
        double vz_ = children[pos]->velocityZ;
        double charge_ = children[pos]->charge;
        double mass_ = children[pos]->mass;
        std::string alias_ = children[pos]->alias;

        delete children[pos];
        children[pos] = nullptr;

        if (pos == o1)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, minPoints->z, midX, midY, midZ);
        }
        else if (pos == o2)
        {
            children[pos] = new Octree(midX, minPoints->y, minPoints->z, maxPoints->x, midY, midZ);
        }
        else if (pos == o3)
        {
            children[pos] = new Octree(minPoints->x, midY, minPoints->z, midX, maxPoints->y, midZ);
        }
        else if (pos == o4)
        {
            children[pos] = new Octree(midX, midY, minPoints->z, maxPoints->x, maxPoints->y, midZ);
        }
        else if (pos == o5)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, midZ, midX, midY, maxPoints->z);
        }
        else if (pos == o6)
        {
            children[pos] = new Octree(midX, minPoints->y, midZ, maxPoints->x, midY, maxPoints->z);
        }
        else if (pos == o7)
        {
            children[pos] = new Octree(minPoints->x, midY, midZ, midX, maxPoints->y, maxPoints->z);
        }
        else if (pos == o8)
        {
            children[pos] = new Octree(midX, midY, midZ, maxPoints->x, maxPoints->y, maxPoints->z);
        }

        children[pos]->insert(root, alias_, x_, y_, z_, vx_, vy_, vz_, mass_, charge_);
        children[pos]->insert(root, octree);
    }
}

void Octree::insert(Octree *root, std::string alias, double x, double y, double z, double vx, double vy, double vz, double mass, double charge)
{
    insert(root, new Octree(alias, x, y, z, vx, vy, vz, mass, charge));

    // if (find(x, y, z))
    // {
    //     std::cout << "point already exists" << std::endl;
    //     return;
    // }

    // if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z)
    // {
    //     // std::cout << "Out of bound" << std::endl;
    //     return;
    // }

    // double midX = (minPoints->x + maxPoints->x) / 2;
    // double midY = (minPoints->y + maxPoints->y) / 2;
    // double midZ = (minPoints->z + maxPoints->z) / 2;
    // double pos = -1;

    // if (x <= midX)
    // {
    //     if (y <= midY)
    //     {
    //         if (z <= midZ)
    //         {
    //             pos = o1;
    //         }
    //         else
    //         {
    //             pos = o5;
    //         }
    //     }
    //     else
    //     {
    //         if (z <= midZ)
    //         {
    //             pos = o3;
    //         }
    //         else
    //         {
    //             pos = o7;
    //         }
    //     }
    // }
    // else
    // {
    //     if (y <= midY)
    //     {
    //         if (z <= midZ)
    //         {
    //             pos = o2;
    //         }
    //         else
    //         {
    //             pos = o6;
    //         }
    //     }
    //     else
    //     {
    //         if (z <= midZ)
    //         {
    //             pos = o4;
    //         }
    //         else
    //         {
    //             pos = o8;
    //         }
    //     }
    // }

    // if (children[pos]->point == nullptr)
    // {
    //     children[pos]->insert(root, alias, x, y, z, vx, vy, vz, mass, charge);
    // }
    // else if (children[pos]->point->x == -1)
    // {
    //     delete children[pos];
    //     children[pos] = new Octree(alias, x, y, z, vx, vy, vz, mass, charge);
    // }
    // else
    // {
    //     double x_ = children[pos]->point->x;
    //     double y_ = children[pos]->point->y;
    //     double z_ = children[pos]->point->z;
    //     double vx_ = children[pos]->velocityX;
    //     double vy_ = children[pos]->velocityY;
    //     double vz_ = children[pos]->velocityZ;
    //     double charge_ = children[pos]->charge;
    //     double mass_ = children[pos]->mass;
    //     std::string alias_ = children[pos]->alias;

    //     delete children[pos];
    //     children[pos] = nullptr;

    //     if (pos == o1)
    //     {
    //         children[pos] = new Octree(minPoints->x, minPoints->y, minPoints->z, midX, midY, midZ);
    //     }
    //     else if (pos == o2)
    //     {
    //         children[pos] = new Octree(midX, minPoints->y, minPoints->z, maxPoints->x, midY, midZ);
    //     }
    //     else if (pos == o3)
    //     {
    //         children[pos] = new Octree(minPoints->x, midY, minPoints->z, midX, maxPoints->y, midZ);
    //     }
    //     else if (pos == o4)
    //     {
    //         children[pos] = new Octree(midX, midY, minPoints->z, maxPoints->x, maxPoints->y, midZ);
    //     }
    //     else if (pos == o5)
    //     {
    //         children[pos] = new Octree(minPoints->x, minPoints->y, midZ, midX, midY, maxPoints->z);
    //     }
    //     else if (pos == o6)
    //     {
    //         children[pos] = new Octree(midX, minPoints->y, midZ, maxPoints->x, midY, maxPoints->z);
    //     }
    //     else if (pos == o7)
    //     {
    //         children[pos] = new Octree(minPoints->x, midY, midZ, midX, maxPoints->y, maxPoints->z);
    //     }
    //     else if (pos == o8)
    //     {
    //         children[pos] = new Octree(midX, midY, midZ, maxPoints->x, maxPoints->y, maxPoints->z);
    //     }

    //     children[pos]->insert(root, alias_, x_, y_, z_, vx_, vy_, vz_, mass_, charge_);
    //     children[pos]->insert(root, alias, x, y, z, vx, vy, vz, mass, charge);
    // }
}

// Recalculates the following properties for each node in the octree
// 1. Center of charge
// 2. Potential energy stores (only does the summation of the stores, not the calculation)
// 3. Kinetic energy stores
void Octree::recalculateParentParameters(Octree *octree)
{
    if (octree->children.size() == 0)
    {
        // if the size of the children is 0, then we have reached a leaf node
        // coc is the centre of charge of the point

        // octree->coc = octree->point;
        if (octree->charge >= 0)
        {
            octree->positiveCoc = octree->point;
        }
        else
        {
            octree->negativeCoc = octree->point;
        }

        // calculate potential energy

        // calculate kinetic energy
        octree->kineticEnergy = 0.5 * octree->mass * (octree->velocityX * octree->velocityX + octree->velocityY * octree->velocityY + octree->velocityZ * octree->velocityZ);

        return;
    }
    else
    {
        // if there are children, we take the center of mass of the children
        // using the formula for center of mass, where
        // coordinate = m1x1 + m2x2 + ... / m1 + m2 + ...

        double xPosPositiveChargeSum = 0;
        double yPosPositiveChargeSum = 0;
        double zPosPositiveChargeSum = 0;

        double xPosNegativeChargeSum = 0;
        double yPosNegativeChargeSum = 0;
        double zPosNegativeChargeSum = 0;

        double massSum = 0;
        double chargeSum = 0;
        double positiveChargeSum = 0;
        double negativeChargeSum = 0;

        double potentialEnergySum = 0;
        double kineticEnergySum = 0;

        double xVelSum = 0;
        double yVelSum = 0;
        double zVelSum = 0;

        double numChildren = 0;

        for (Octree *child : octree->children)
        {
            if (child != nullptr || child->point->x != -1)
            {
                recalculateParentParameters(child);

                massSum += child->mass;

                chargeSum += child->charge;
                if (child->charge >= 0)
                {
                    positiveChargeSum += child->charge;

                    if (child->positiveCoc->x != -1)
                    {
                        xPosPositiveChargeSum += child->positiveCoc->x * child->charge;
                        yPosPositiveChargeSum += child->positiveCoc->y * child->charge;
                        zPosPositiveChargeSum += child->positiveCoc->z * child->charge;
                    }
                }
                else
                {
                    negativeChargeSum += child->charge;

                    if (child->negativeCoc->x != -1)
                    {
                        xPosNegativeChargeSum += child->negativeCoc->x * child->charge;
                        yPosNegativeChargeSum += child->negativeCoc->y * child->charge;
                        zPosNegativeChargeSum += child->negativeCoc->z * child->charge;
                    }
                }

                // calculate total velocity
                xVelSum += child->velocityX;
                yVelSum += child->velocityY;
                zVelSum += child->velocityZ;

                // calculate kinetic energy
                kineticEnergySum += 0.5 * child->mass * (child->velocityX * child->velocityX + child->velocityY * child->velocityY + child->velocityZ * child->velocityZ);
                potentialEnergySum += child->potentialEnergy;
            }
        }

        octree->charge = chargeSum;
        octree->negativeCharge = negativeChargeSum;
        octree->positiveCharge = positiveChargeSum;

        octree->mass = massSum;
        octree->velocityX = xVelSum;
        octree->velocityY = yVelSum;
        octree->velocityZ = zVelSum;

        octree->potentialEnergy = potentialEnergySum;
        octree->kineticEnergy = kineticEnergySum;

        // Set centre of charges for positive and negative charges
        if (positiveChargeSum != 0)
        {
            octree->positiveCoc = new Point(xPosPositiveChargeSum / positiveChargeSum, yPosPositiveChargeSum / positiveChargeSum, zPosPositiveChargeSum / positiveChargeSum);
        }
        else
        {
            octree->positiveCoc = new Point(-1, -1, -1);
        }

        if (negativeChargeSum != 0)
        {
            octree->negativeCoc = new Point(xPosNegativeChargeSum / negativeChargeSum, yPosNegativeChargeSum / negativeChargeSum, zPosNegativeChargeSum / negativeChargeSum);
        }
        else
        {
            octree->negativeCoc = new Point(-1, -1, -1);
        }
    }
}