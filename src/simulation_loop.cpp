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

void Simulation::mainLoop(Octree *volume, int iterations, float timeStep)
{ // simulation volume + iterations (-1 for infinite iterations) + time step (i.e how many seconds are in each iteration)
  // simulation loop

    Octree newOctree = Octree(
        volume->minPoints->x,
        volume->minPoints->y,
        volume->minPoints->z,
        volume->maxPoints->x,
        volume->maxPoints->y,
        volume->maxPoints->z);

    Octree *newOctree_ptr = &newOctree;

    std::vector<Octree *> childrenList = getChildren(volume);

    for (Octree *child : childrenList)
    {
        float x = child->point->x + child->velocityX * timeStep;
        float y = child->point->y + child->velocityY * timeStep;
        float z = child->point->z + child->velocityZ * timeStep;

        float vx = child->velocityX + child->forceX / child->mass * timeStep;
        float vy = child->velocityY + child->forceY / child->mass * timeStep;
        float vz = child->velocityZ + child->forceZ / child->mass * timeStep;

        newOctree.insert(
            newOctree_ptr,
            x,
            y,
            z,
            vx,
            vy,
            vz,
            child->mass,
            child->charge);
    }
    newOctree.recalculateCenterOfCharge(newOctree_ptr);
    volume = newOctree_ptr;
    float a = 1;
}