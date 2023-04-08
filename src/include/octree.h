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

struct Point {
    int x;
    int y;
    int z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(int a, int b, int c) : x(a), y(b), z(c) {}
};

class Octree {
    Point* point;
    Point *minPoints, *maxPoints;
    std::vector<Octree*> children;

    public:
        Octree();
        Octree(int x, int y, int z);
        Octree(int minX, int minY, int minZ, int maxX, int maxY, int maxZ);
           
        bool find(int x, int y, int z);
        void insert(int x, int y, int z);
};