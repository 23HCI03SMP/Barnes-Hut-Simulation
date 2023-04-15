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

Octree::Octree(float mass)
{
    point = new Point();

    this->mass = mass;
}

Octree::Octree(int x, int y, int z, float mass)
{
    point = new Point(x, y, z);

    this->mass = mass;
}

// Initializing a base octree
Octree::Octree(int minX, int minY, int minZ, int maxX, int maxY, int maxZ, float mass)
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
        // Since this is the base octree, there is no parent
        children[i] = new Octree(mass);
    }
}

bool Octree::find(int x, int y, int z)
{
    // If point is out of bound
    if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z)
        return 0;

    // Otherwise perform binary search
    // for each ordinate
    int midx = (minPoints->x + maxPoints->x) / 2;
    int midy = (minPoints->y + maxPoints->y) / 2;
    int midz = (minPoints->z + maxPoints->z) / 2;
    int pos = -1;

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
    if (children[pos] == nullptr)
    {
        return children[pos]->find(x, y, z);
    }

    // If an empty node is encountered
    if (children[pos]->point->x == -1)
    {
        return 0;
    }

    // If node is found with
    // the given value
    if (x == children[pos]->point->x && y == children[pos]->point->y && z == children[pos]->point->z)
        return 1;

    return 0;
}

void Octree::insert(Octree *&root, int x, int y, int z, float mass)
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

    int midX = (minPoints->x + maxPoints->x) / 2;
    int midY = (minPoints->y + maxPoints->y) / 2;
    int midZ = (minPoints->z + maxPoints->z) / 2;
    int pos = -1;

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
        children[pos]->insert(children[pos], x, y, z, mass);
    }
    else if (children[pos]->point->x == -1)
    {
        delete children[pos];
        children[pos] = new Octree(x, y, z, mass);

        recalculateCenterOfMass(root);
    }
    else
    {
        int x_ = children[pos]->point->x;
        int y_ = children[pos]->point->y;
        int z_ = children[pos]->point->z;
        delete children[pos];
        children[pos] = nullptr;

        if (pos == o1)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, minPoints->z, midX, midY, midZ, mass);
        }
        else if (pos == o2)
        {
            children[pos] = new Octree(midX + 1, minPoints->y, minPoints->z, maxPoints->x, midY, midZ, mass);
        }
        else if (pos == o3)
        {
            children[pos] = new Octree(minPoints->x, midY + 1, minPoints->z, midX, maxPoints->y, midZ, mass);
        }
        else if (pos == o4)
        {
            children[pos] = new Octree(midX + 1, midY + 1, minPoints->z, maxPoints->x, maxPoints->y, midZ, mass);
        }
        else if (pos == o5)
        {
            children[pos] = new Octree(minPoints->x, minPoints->y, midZ + 1, midX, midY, maxPoints->z, mass);
        }
        else if (pos == o6)
        {
            children[pos] = new Octree(midX + 1, minPoints->y, midZ + 1, maxPoints->x, midY, maxPoints->z, mass);
        }
        else if (pos == o7)
        {
            children[pos] = new Octree(minPoints->x, midY + 1, midZ + 1, midX, maxPoints->y, maxPoints->z, mass);
        }
        else if (pos == o8)
        {
            children[pos] = new Octree(midX + 1, midY + 1, midZ + 1, maxPoints->x, maxPoints->y, maxPoints->z, mass);
        }

        children[pos]->insert(children[pos], x_, y_, z_, mass);
        recalculateCenterOfMass(root);
    }
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

        float xPosSum;
        float yPosSum;
        float zPosSum;
        
        float massSum;

        for(Octree* child : octree->children)
        {
            if (child->point->x != -1)
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

    // else
    // {
    //     // if there are children, we take the center of mass of the children
    //     // using the formula for center of mass, where
    //     // coordinate = m1x1 + m2x2 + ... / m1 + m2 + ...

    //     float xPosSum;
    //     float yPosSum;
    //     float zPosSum;
    //     float xMassSum;
    //     float yMassSum;
    //     float zMassSum;

    //     for (int i = 0; i <= children.size(), i++;)
    //     {
    //         if (children[i]->point->x != -1)
    //         {
    //             Octree *child = children[i];

    //             xMassSum += child->mass;
    //             yMassSum += child->mass;
    //             zMassSum += child->mass;

    //             xPosSum += child->com->x * child->mass;
    //             yPosSum += child->com->y * child->mass;
    //             zPosSum += child->com->z * child->mass;
    //         }
    //     }

    //     octree->com = new Point(xPosSum / xMassSum, yPosSum / yMassSum, zPosSum / zMassSum);
    // }
}