#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Pi constant
constexpr double pi() { return std::acos(-1); }

std::vector<double> randUnitVector(int d, gsl_rng *r);
std::vector<std::array<int, 3>> plummer(int Npart, double a, double m, double G, int seed);
std::vector<int> plummerDist_3d_xyz(int Npart, double a, int seed);

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

private:
    void recalculateCenterOfMass(Octree *&octree);

public:
    Point *point;
    Point *minPoints, *maxPoints;
    Point *com;

    double forceX = 0, forceY = 0, forceZ = 0;

    float mass = 0; // Need to set mass to zero because default value of floating point is some random negative number
    std::vector<Octree *> children;

    Octree(float mass);
    Octree(int x, int y, int z, float mass);
    Octree(int minX, int minY, int minZ, int maxX, int maxY, int maxZ, float mass);

    void insert(Octree *&root, int x, int y, int z, float mass);
    bool find(int x, int y, int z);
};

class Barnes
{
    private:
        bool isExternalNode(Octree *&octree);
    public:
        void calcForce(Octree *&node, Octree *&b, float thetaLimit);

};