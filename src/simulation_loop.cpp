#include <iostream>
#include "include/barnesHut.h"

std::vector<Octree *&> childrenList;

int mainLoop(Octree *&volume, int iterations, double timeStep) { //simulation volume + iterations (-1 for infinite iterations) + time steo (i.e how many seconds are in each iteration)
    //simulation loop
    for (int i = 0; i < iterations; i++) {

        Octree newOctree = Octree(volume->minPoints->x, volume->minPoints->y, volume->minPoints->z, volume->maxPoints->x, volume->maxPoints->y, volume->maxPoints->z);
        Octree *newOctreePtr = &newOctree;

        for (Octree*& child : childrenList) {
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
}

std::vector<Octree *&> getChildren(Octree *&volume) {
    for (Octree*& child : volume->children) {
        childrenList.push_back(child);
        if (child->children.size() == 0) {
            return;
        }
        getChildren(child);
    }
}