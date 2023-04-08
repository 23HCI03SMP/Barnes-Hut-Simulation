#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>

// Pi constant
constexpr double pi() { return std::acos(-1); }

std::vector<double> randUnitVector(int d, gsl_rng *r);
std::vector<std::array<int, 3>> plummer(int Npart, double a, double m, double G, int seed);
std::vector<int> plummerDist_3d_xyz(int Npart, double a, int seed);

class Particle {
    public:
        double position[3]= {};
};

class Bbox {
    public:
        std::vector<std::vector<double>> box = {
            {-1, 1},
            {-1, 1},
            {-1, 1}
        };
        std::vector<Bbox> children;

        Particle particle;
        Particle com; // center of mass

        double xLow = box[0][0];
        double xMax = box[0][1];
        double yLow = box[1][0];
        double yMax = box[1][1];
        double zLow = box[2][0];
        double zMax = box[2][1];
        double centre[3] = {((xLow + xMax)/2), ((yLow, yMax)/2), ((zLow, zMax)/2)};
        
        int n = 0; //number of particles
};

class Node {
    //min and max points (2x2x2 box)
    public:
        void insertParticle(Particle particle, Bbox box);
        void createChildren(Bbox box);
        void updateCom(Bbox box);
};