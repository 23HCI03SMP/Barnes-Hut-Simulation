#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>
#include "include/barnesHut.h"

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

Octree::Octree(float x, float y, float z, float mass)
{
    point = new Point(x, y, z);

    this->mass = mass;
}

// Initializing a base octree
Octree::Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
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

bool Octree::find(float x, float y, float z)
{
    // If point is out of bound
    if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z)
        return false;

    // Otherwise perform binary search
    // for each ordinate
    float midx = (minPoints->x + maxPoints->x) / 2;
    float midy = (minPoints->y + maxPoints->y) / 2;
    float midz = (minPoints->z + maxPoints->z) / 2;
    float pos = -1;

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
    // if (children[pos]->point->x == -1)
    // {
    //     return false;
    // }

    // If node is found with
    // the given value
    if (x == children[pos]->point->x && y == children[pos]->point->y && z == children[pos]->point->z)
        return true;

    return false;
}

void Octree::insert(Octree *&root, float x, float y, float z, float mass)
{
    if (find(x, y, z))
    {
        std::cout << "point already exists" << std::endl;
        return;
    }

    if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z)
    {
        std::cout << "Out of bound" << std::endl;
        return;
    }

    float midX = (minPoints->x + maxPoints->x) / 2;
    float midY = (minPoints->y + maxPoints->y) / 2;
    float midZ = (minPoints->z + maxPoints->z) / 2;
    float pos = -1;

    if (x <= midX)
    {
        if (y <= midY)
        {
            if (z <= midZ)
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
            if (z <= midZ)
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
        if (y <= midY)
        {
            if (z <= midZ)
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
            if (z <= midZ)
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
        children[pos]->insert(root, x, y, z, mass);
    }
    else if (children[pos]->point->x == -1)
    {
        delete children[pos];
        children[pos] = new Octree(x, y, z, mass);
    }
    else
    {
        float x_ = children[pos]->point->x;
        float y_ = children[pos]->point->y;
        float z_ = children[pos]->point->z;
        float mass_ = children[pos]->mass;

        
        delete children[pos];
        children[pos] = nullptr;

        if (pos == o1)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, minPoints->z, midX, midY, midZ);
        }
        else if (pos == o2)
        {
            children[pos] = new Octree(midX + 1, minPoints->y, minPoints->z, maxPoints->x, midY, midZ);
        }
        else if (pos == o3)
        {
            children[pos] = new Octree(minPoints->x, midY + 1, minPoints->z, midX, maxPoints->y, midZ);
        }
        else if (pos == o4)
        {
            children[pos] = new Octree(midX + 1, midY + 1, minPoints->z, maxPoints->x, maxPoints->y, midZ);
        }
        else if (pos == o5)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, midZ + 1, midX, midY, maxPoints->z);
        }
        else if (pos == o6)
        {
            children[pos] = new Octree(midX + 1, minPoints->y, midZ + 1, maxPoints->x, midY, maxPoints->z);
        }
        else if (pos == o7)
        {
            children[pos] = new Octree(minPoints->x, midY + 1, midZ + 1, midX, maxPoints->y, maxPoints->z);
        }
        else if (pos == o8)
        {
            children[pos] = new Octree(midX + 1, midY + 1, midZ + 1, maxPoints->x, maxPoints->y, maxPoints->z);
        }

        children[pos]->insert(root, x_, y_, z_, mass_);
        children[pos]->insert(root, x, y, z, mass);
    }

    recalculateCenterOfMass(root);
}

void Octree::recalculateCenterOfMass(Octree *&octree)
{
    if (octree->children.size() == 0)
    {
        // if the size of the children is 0, then we have reached a leaf node
        // com is the center of mass of the particle at the leaf node
        octree->com = octree->point;

        return;
    }
    else
    {
        // if there are children, we take the center of mass of the children
        // using the formula for center of mass, where
        // coordinate = m1x1 + m2x2 + ... / m1 + m2 + ...

        float xPosSum = 0;
        float yPosSum = 0;
        float zPosSum = 0;

        float massSum = 0;

        for (Octree *child : octree->children)
        {
            if (child != nullptr || child->point->x != -1)
            {
                recalculateCenterOfMass(child);

                massSum += child->mass;

                xPosSum += child->com->x * child->mass;
                yPosSum += child->com->y * child->mass;
                zPosSum += child->com->z * child->mass;
            }
        }

        octree->mass = massSum;
        octree->com = new Point(xPosSum / massSum, yPosSum / massSum, zPosSum / massSum);
    }
}