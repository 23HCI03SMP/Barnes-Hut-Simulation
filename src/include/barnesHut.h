#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Pi constant
constexpr double PI = 3.14159265358979323846;

struct Point
{
    int x;
    int y;
    int z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(int a, int b, int c) : x(a), y(b), z(c) {}
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
    Octree(int x, int y, int z, float mass);
    Octree(int minX, int minY, int minZ, int maxX, int maxY, int maxZ, float mass);

    void insert(Octree *&root, int x, int y, int z, float mass);
    bool find(int x, int y, int z);
};