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

void Simulation::mainLoop(Octree *&volume, double timeStep)
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

        double Px = child->magneticFieldX * timeStep / child->mass;
        double Py = child->magneticFieldY * timeStep / child->mass;
        double Pz = child->magneticFieldZ * timeStep / child->mass;

        double x = child->point->x + child->velocityX * timeStep;
        double y = child->point->y + child->velocityY * timeStep;
        double z = child->point->z + child->velocityZ * timeStep;

        Eigen::Matrix3d A
        {
            {0, -Py, Py},
            {Pz, 0, -Px},
            {-Py, Px, 0}
        };

        Eigen::Matrix3d I = Eigen::Matrix3d::Identity();

        Eigen::Matrix3d IA = (I + A).inverse();

        Eigen::Vector3d Vprime = IA * (I - A) * Eigen::Vector3d(child->velocityX, child->velocityY, child->velocityZ);

        double vx = 0, vy = 0, vz = 0;
        vx = Vprime(0); 
        vx += ((child->charge * child->electricFieldX) / child->mass * timeStep);
        vy = Vprime(1); 
        vy += ((child->charge * child->electricFieldY) / child->mass * timeStep);
        vz = Vprime(2);
        vz += ((child->charge * child->electricFieldZ) / child->mass * timeStep);

        // output velocity to file.txt
        // std::ofstream velocityFile;
        // velocityFile.open(std::filesystem::current_path() / "velocity.txt", std::ios_base::app);
        // velocityFile << "vx: " << vx << " vy: " << vy << " vz: " << vz << std::endl;

        if (!MagneticField && !ElectricField)
        {
            vx = child->velocityX;
            vy = child->velocityY;
            vz = child->velocityZ;
        }

        // output positions to positions.txt
        // std::ofstream positionFile;
        // positionFile.open(std::filesystem::current_path() / "position.txt", std::ios_base::app);
        // positionFile << "x: " << x << " y: " << y << " z: " << z << std::endl;

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