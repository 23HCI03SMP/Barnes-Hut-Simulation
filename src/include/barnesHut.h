#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Pi constant
constexpr float PI = 3.14159265358979323846;

// Coulomb's constant
constexpr float K = 8.9875517923e9;

struct Point
{
    float x;
    float y;
    float z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(float a, float b, float c) : x(a), y(b), z(c) {}
};

class Octree
{

private:
    void recalculateCenterOfMass(Octree *&octree);

public:
    Point *point;
    Point *minPoints, *maxPoints;
    Point *com;

    double forceX = 0, forceY = 0, forceZ = 0;

    float mass = 0; // Need to set mass to zero because default value of floating point is some random negative number
    std::vector<Octree *> children;

    Octree();
    Octree(float x, float y, float z, float mass);
    Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    void insert(Octree *&root, float x, float y, float z, float mass);
    bool find(float x, float y, float z);
};

class Barnes
{
    private:
        bool isExternalNode(Octree *&octree);
    public:
        void calcForce(Octree *&node, Octree *&b, float thetaLimit);

};