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

//o represents octet
#define o1 0
#define o2 1
#define o3 3
#define o4 2
#define o5 4
#define o6 5
#define o7 7
#define o8 6

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
        Octree() {
            point = new Point();
        }
        Octree(int x, int y, int z) {
            point = new Point(x, y, z);
        }
        Octree(int minX, int minY, int minZ, int maxX, int maxY, int maxZ) {
            if (maxX < minX || maxY < minY || maxZ < minZ) {
                std::cout << "Invalid Inputs";
                return;
            }
            point = nullptr;
            minPoints = new Point(minX, minY, minZ);
            maxPoints = new Point(maxX, maxY, maxZ);

            children.assign(8, nullptr);
            for (int i = o1; i <= o7; ++i) {
                children[i] = new Octree();
            }
        }
        
        bool find(int x, int y, int z)
        {
            // If point is out of bound
            if (x < minPoints->x
                || x > maxPoints->x
                || y < minPoints->y
                || y > maxPoints->y
                || z < minPoints->z
                || z > maxPoints->z)
                return 0;
    
            // Otherwise perform binary search
            // for each ordinate
            int midx = (minPoints->x
                        + maxPoints->x)
                    / 2;
            int midy = (minPoints->y
                        + maxPoints->y)
                    / 2;
            int midz = (minPoints->z
                        + maxPoints->z)
                    / 2;
    
            int pos = -1;
    
            // Deciding the position
            // where to move
            if (x <= midx) {
                if (y <= midy) {
                    if (z <= midz)
                        pos = o1;
                    else
                        pos = o5;
                }
                else {
                    if (z <= midz)
                        pos = o3;
                    else
                        pos = o7;
                }
            }
            else {
                if (y <= midy) {
                    if (z <= midz)
                        pos = o2;
                    else
                        pos = o6;
                }
                else {
                    if (z <= midz)
                        pos = o4;
                    else
                        pos = o8;
                }
            }
            
            // If an internal node is encountered
            if (children[pos]->point == nullptr) {
                return children[pos]->find(x, y, z);
            }
    
            // If an empty node is encountered
            else if (children[pos]->point->x == -1) {
                return 0;
            }
            else {
    
                // If node is found with
                // the given value
                if (x == children[pos]->point->x
                    && y == children[pos]->point->y
                    && z == children[pos]->point->z)
                    return 1;
            }
            return 0;
        }

        void insert(int x, int y, int z) {
            Octree octree;
            if (octree.find(x, y, x)) {
                std::cout << "point already exists" << std::endl;
                return;
            }

            if (x < minPoints->x || x > maxPoints->x || y < minPoints->y || y > maxPoints->y || z < minPoints->z || z > maxPoints->z) {
                    std::cout << "Out of bound" << std::endl;
                    return;
            }

            
        }
};