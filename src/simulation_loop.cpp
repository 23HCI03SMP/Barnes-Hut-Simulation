#include <iostream>
#include "include/barnesHut.h"
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <filesystem>


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
        bool MagneticField = !(child->magneticFieldX == 0 && child->magneticFieldY == 0 && child->magneticFieldZ == 0);
        bool ElectricField = !(child->electricFieldX == 0 && child->electricFieldY == 0 && child->electricFieldZ == 0);

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

        float vx = 0, vy = 0, vz = 0;
        vx = Vprime(0); 
        vx += ((child->charge * child->electricFieldX) / child->mass * timeStep);
        vy = Vprime(1); 
        vy += ((child->charge * child->electricFieldY) / child->mass * timeStep);
        vz = Vprime(2);
        vz += ((child->charge * child->electricFieldZ) / child->mass * timeStep);

        if (!MagneticField && !ElectricField)
        {
            vx = child->velocityX;
            vy = child->velocityY;
            vz = child->velocityZ;
        }

        Octree* newChild = new Octree(
            child->alias,
            x,
            y,
            z,
            vx,
            vy,
            vz,
            child->mass,
            child->charge);
        
        // because potential energy is calculated in the calcForce function, we need to copy it over
        newChild->potentialEnergy = child->potentialEnergy;

        newOctree->insert(newOctree, newChild);
    }

    newOctree->recalculateParentParameters(newOctree);

    volume = newOctree;

    // output average kinetic energy
    std::cout << "Average kinetic energy: " << volume->kineticEnergy / volume->mass << std::endl;

    // output kinetic energy
    // std::cout << "Kinetic energy: " << volume->kineticEnergy << std::endl;
    // std::cout << "Potential energy: " << volume->potentialEnergy << std::endl;

    // std::cout << "Sum of kinetic and potential energy: " << volume->kineticEnergy + volume->potentialEnergy << std::endl;
    
    // output ke, pe and sum to energy.txt file
    std::ofstream energyFile;
    energyFile.open(std::filesystem::current_path() / "energy.txt", std::ios::app);
    energyFile << volume->kineticEnergy << "," << volume->potentialEnergy << "," << volume->kineticEnergy + volume->potentialEnergy << std::endl;
    energyFile.close();
}