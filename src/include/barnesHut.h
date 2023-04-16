#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Pi constant
constexpr float PI = 3.14159265358979323846;

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
    Point *point;
    Point *minPoints, *maxPoints;
    Point *com;

    std::vector<Octree *> children;

    float mass = 0; // Need to set mass to zero because default value of floating point is some random negative number

private:
    void recalculateCenterOfMass(Octree *&octree);

public:
    Octree(float mass);
    Octree(float x, float y, float z, float mass);
    Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float mass);

    void insert(Octree *&root, float x, float y, float z, float mass);
    bool find(float x, float y, float z);
};