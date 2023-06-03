#include <iostream>
#include "include/barnesHut.h"

std::vector<Octree *> getChildren(Octree *volume)
{
    std::vector<Octree *> childrenList;
    for (Octree *child : volume->children)
    {
        if (child->mass != 0 && child->charge != 0 && child->point != nullptr)
        {
            childrenList.push_back(child);
        }
        std::vector<Octree *> recursChildren = getChildren(child);
        childrenList.insert(childrenList.end(), recursChildren.begin(), recursChildren.end());
    }
    return childrenList;
}

std::vector<Octree *> getNodes(Octree *volume)
{
    std::vector<Octree *> childrenList;
    for (Octree *child : volume->children)
    {
        if (child->mass != 0)
        {
            childrenList.push_back(child);
        }
        std::vector<Octree *> recursChildren = getChildren(child);
        childrenList.insert(childrenList.end(), recursChildren.begin(), recursChildren.end());
    }
    return childrenList;
}

Octree Simulation::mainLoop(Octree *volume, int iterations, float timeStep)
{ // simulation volume + iterations (-1 for infinite iterations) + time step (i.e how many seconds are in each iteration)
    // simulation loop

    Octree newOctree = Octree(
        volume->minPoints->x,
        volume->minPoints->y,
        volume->minPoints->z,
        volume->maxPoints->x,
        volume->maxPoints->y,
        volume->maxPoints->z);

    Octree *newOctreePtr = &newOctree;

    for (int i = 0; i < iterations; i++)
    {
        std::vector<Octree *> childrenList = getChildren(volume);
        for (Octree *child : childrenList)
        {
            float pointX = child->point->x + child->velocityX * timeStep;
            float pointY = child->point->y + child->velocityY * timeStep;
            float pointZ = child->point->z + child->velocityZ * timeStep;

            float velocityX = child->velocityX + child->forceX / child->mass * timeStep;
            float velocityY = child->velocityY + child->forceY / child->mass * timeStep;
            float velocityZ = child->velocityZ + child->forceZ / child->mass * timeStep;

            // update simulation volume
            newOctree.insert(
                newOctreePtr,
                pointX,
                pointY,
                pointZ,
                velocityX,
                velocityY,
                velocityZ,
                child->mass,
                child->charge);
            newOctree.recalculateCenterOfCharge(newOctreePtr);
        }
    }

    newOctree.recalculateCenterOfCharge(newOctreePtr);

    return newOctree;
}