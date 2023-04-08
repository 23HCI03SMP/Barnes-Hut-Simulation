#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>

// Pi constant
constexpr double pi() { return std::acos(-1); }

std::vector<double> randUnitVector(int d, gsl_rng *r);
std::vector<std::array<int, 3>> plummer(int Npart, float a, float m, float G, int seed);
std::vector<int> plummerDist_3d_xyz(int Npart, float a, int seed);

class Particle {
    public:
        float position[3]= {};
};

class Bbox {
    public:
        std::vector<std::vector<float>> box = {
            {-1, 1},
            {-1, 1},
            {-1, 1}
        };
        std::vector<Bbox> children;

        Particle particle;
        Particle com; // center of mass

        float xLow = box[0][0];
        float xMax = box[0][1];
        float yLow = box[1][0];
        float yMax = box[1][1];
        float zLow = box[2][0];
        float zMax = box[2][1];
        float centre[3] = {(xLow + xMax)/2, (yLow, yMax)/2, (zLow, zMax)/2};
        
        int n = 0; //number of particles
};

class Node {
    //min and max points (2x2x2 box)
    public:
        void insertParticle(Particle particle, Bbox box);
        void createChildren(Bbox box);
        void updateCom(Bbox box);
};