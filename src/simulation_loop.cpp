#include <iostream>
#include "include/barnesHut.h"

std::vector<Octree *> getChildren(Octree *volume)
{
    std::vector<Octree *> childrenList;
    for (Octree *child : volume->children)
    {
        if (child->children.size() == 0 && child->point != nullptr && child->point->x != -1)
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
            child->point->x += child->velocityX * timeStep;
            child->point->y += child->velocityY * timeStep;
            child->point->z += child->velocityZ * timeStep;

            child->velocityX += child->forceX / child->mass * timeStep;
            child->velocityY += child->forceY / child->mass * timeStep;
            child->velocityZ += child->forceZ / child->mass * timeStep;

            // update simulation volume
            newOctree.insert(
                newOctreePtr,
                child->point->x,
                child->point->y,
                child->point->z,
                child->velocityX,
                child->velocityY,
                child->velocityZ,
                child->mass,
                child->charge,
                child->alias);
        }
    }
    return newOctree;
}