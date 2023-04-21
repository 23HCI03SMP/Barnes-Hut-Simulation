#include <iostream>
#include "include/barnesHut.h"

std::vector<Octree *> getChildren(Octree *&volume) {
    std::vector<Octree *> childrenList;
    for (Octree* child : volume->children) {
        childrenList.push_back(child);
        if (child->children.size() == 0) {
            break;
        }
        getChildren(child);
    }
    return childrenList;
}

Octree mainLoop(Octree *&volume, int iterations, double timeStep) { //simulation volume + iterations (-1 for infinite iterations) + time step (i.e how many seconds are in each iteration)
    //simulation loop
    Octree newOctree = Octree(volume->minPoints->x, volume->minPoints->y, volume->minPoints->z, volume->maxPoints->x, volume->maxPoints->y, volume->maxPoints->z);
    Octree *newOctreePtr = &newOctree;
    for (int i = 0; i < iterations; i++) {
        std::vector<Octree *> childrenList = getChildren(volume);
        for (Octree *child : childrenList) {
            //update accelerations
            double accelX = child->forceX/child->mass;
            double accelY = child->forceY/child->mass;
            double accelZ = child->forceZ/child->mass;

            //calculate displacement using suvat
            double sX = child->velocityX * timeStep + (0.5 * accelX * timeStep * timeStep);
            double sY = child->velocityY * timeStep + (0.5 * accelY * timeStep * timeStep);
            double sZ = child->velocityZ * timeStep + (0.5 * accelZ * timeStep * timeStep);

            child->point->x += sX;
            child->point->y += sY;
            child->point->z += sZ;

            //update simulation volume
            newOctree.insert(newOctreePtr, child->point->x, child->point->y, child->point->z, child->charge, child->mass);
        }
    }
    return newOctree;
}