#include <iostream>
#include "include/barnesHut.h"
#include <eigen3/Eigen/Dense>


//Gets all particles in octree
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

//Gets all nodes in octree
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

void Simulation::mainLoop(Octree *&volume, float timeStep)
{
    Octree *newOctree = new Octree(
        volume->minPoints->x,
        volume->minPoints->y,
        volume->minPoints->z,
        volume->maxPoints->x,
        volume->maxPoints->y,
        volume->maxPoints->z);

    std::vector<Octree *> childrenList = getChildren(volume);

    for (Octree *child : childrenList)
    {
        float Px = child->magneticFieldX * timeStep / child->mass;
        float Py = child->magneticFieldY * timeStep / child->mass;
        float Pz = child->magneticFieldZ * timeStep / child->mass;

        float x = child->point->x + child->velocityX * timeStep;
        float y = child->point->y + child->velocityY * timeStep;
        float z = child->point->z + child->velocityZ * timeStep;

        Eigen::Matrix3f A
        {
            {0, -Py, Py},
            {Pz, 0, -Px},
            {-Py, Px, 0}
        };

        Eigen::Matrix3f I = Eigen::Matrix3f::Identity();

        Eigen::Matrix3f IA = (I + A).inverse();

        Eigen::Vector3f Vprime = IA * (I - A) * Eigen::Vector3f(child->velocityX, child->velocityY, child->velocityZ);
        // std::cout << "vx: " << Vprime(0) << std::endl;
        // std::cout << "vy: " << Vprime(1) << std::endl;
        // std::cout << "vz: " << Vprime(2) << std::endl;

        float vx = child->forceX / child->mass * timeStep;
        //vx += Vprime(0);
        float vy = child->forceY / child->mass * timeStep;
        //vy += Vprime(1);
        float vz = child->forceZ / child->mass * timeStep;
        //vz += Vprime(2);

        newOctree->insert(
            newOctree,
            child->alias,
            x,
            y,
            z,
            vx,
            vy,
            vz,
            child->mass,
            child->charge);
    }

    newOctree->recalculateCenterOfCharge(newOctree);

    volume = newOctree;
}