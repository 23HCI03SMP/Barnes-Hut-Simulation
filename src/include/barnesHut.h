#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

#pragma once

// Path to initial conditions csv file
constexpr char INITIAL_VALUES_PATH[] = "initial_values.csv";

// Path to simulation results csv file
constexpr char SIMULATION_VALUES_PATH[] = "simulation_values.csv";

// Path to theta csv file
constexpr char THETA_PATH[] = "theta.csv";

// Pi constant
constexpr double PI = 3.14159265358979323846;

// Coulomb's constant
constexpr double K_E = 8.9875517923e9;
// constexpr double K_E = 9.937e-13;
// constexpr double K_E = 0;
// constexpr double K_E = 2.947877826e33;

// Boltzmann constant (J/K)
// constexpr double K_B = 8.617333e-5;
// constexpr double K_B = 1.380649e-23;
constexpr double K_B = 4.528468561; 

// Biot Savart constant
// constexpr double K_BS = 1e-7;
// constexpr double K_BS = 0;
constexpr double K_BS = 3.279956427e23;

// Headers for values csv file
constexpr char VALUE_FILE_HEADER[] = "x,y,z,vx,vy,vz,mass,charge,alias";

enum Shape
{
    SPHERE,
    REGULAR_CYLINDER,
    HOLLOW_CYLINDER,
};

struct InsertedParticle
{
    std::string alias;
    double mass;
    double charge;
    double percentage;

    InsertedParticle() : alias(""), mass(0), charge(0), percentage(0) {}
    InsertedParticle(std::string alias, double mass, double charge, double percentage) : alias(alias), mass(mass), charge(charge), percentage(percentage) {}
};

struct Point
{
    double x;
    double y;
    double z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(double a, double b, double c) : x(a), y(b), z(c) {}
};

struct CSVPoint
{
    std::string alias;
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
    double mass;
    double charge;

    CSVPoint() : alias(""), x(-1), y(-1), z(-1), vx(-1), vy(-1), vz(-1), mass(-1), charge(-1) {}
    CSVPoint(std::string alias, double x, double y, double z, double vx, double vy, double vz, double mass, double charge) : alias(alias),
                                                                                                                     x(x),
                                                                                                                     y(y),
                                                                                                                     z(z),
                                                                                                                     vx(vx),
                                                                                                                     vy(vy),
                                                                                                                     vz(vz),
                                                                                                                     mass(mass),
                                                                                                                     charge(charge) {}
};

class Octree
{
private:
public:
    Point *point;
    Point *minPoints, *maxPoints;
    Point *positiveCoc = new Point(-1, -1, -1);
    Point *negativeCoc = new Point(-1, -1, -1);

    double forceX = 0, forceY = 0, forceZ = 0;

    double mass = 0;
    double velocityX = 0, velocityY = 0, velocityZ = 0; // current velocity of the particle
    std::vector<Octree *> children;

    // Change values accordingly
    double magneticFieldX = 0;
    double magneticFieldY = 0;
    double magneticFieldZ = 0;

    double electricFieldX = 0;
    double electricFieldY = 0;
    double electricFieldZ = 0;
    // -------------------------

    double charge = 0;         // @attention IMPORTANT: USE THIS VALUE when calculating charge of single particles!
    double positiveCharge = 0; // @attention IMPORTANT: positiveCharge and negativeCharge do not exist on single particles, and only for octrees! DO NOT USE for single particles
    double negativeCharge = 0; // @attention IMPORTANT: positiveCharge and negativeCharge do not exist on single particles, and only for octrees! DO NOT USE for single particles

    double kineticEnergy = 0;
    double potentialEnergy = 0;

    std::string alias;

    Octree();
    Octree(std::string alias, double x, double y, double z, double vx, double vy, double vz, double mass, double charge);
    Octree(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);

    void recalculateParentParameters(Octree *octree);

    void insert(Octree *root, std::string alias, double x, double y, double z, double vx, double vy, double vz, double mass, double charge);
    void insert(Octree *root, Octree *octree);
    bool find(double x, double y, double z);
};

class Barnes
{
public:
    bool isExternalNode(Octree *octree);
    void calcForce(Octree *node, Octree *b, double thetaLimit);
    void addForce(Octree *node, Octree *b, double posdx, double posdy, double posdz, double negdx, double negdy, double negdz);
};

class Simulation
{
private:
public:
    void mainLoop(Octree *&volume, double timeStep);
};

std::vector<CSVPoint> loadInitialValues();
std::vector<CSVPoint> generateInitialPoints(Octree *&octree,
                                            double density,
                                            double temperature,
                                            std::vector<InsertedParticle> particles,
                                            Shape shape,
                                            std::initializer_list<double> dimensions, 
                                            bool isLiner = false,
                                            bool append = false,
                                            bool load = true);

std::vector<Octree *> getChildren(Octree *volume);

void clearSimulationValues();
void writeSimulationValues(std::vector<Octree *> children, std::ofstream &ValueFile);

int tester();