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
constexpr float PI = 3.14159265358979323846;

// Coulomb's constant
// constexpr float K_E = 8.9875517923e9;
// constexpr float K_E = 9.937e-13;
// constexpr float K_E = 0;
constexpr float K_E = 2.948680828e33;

// Boltzmann constant (J/K)
// constexpr float K_B = 8.617333e-5;
// constexpr float K_B = 1.380649e-23;
constexpr float K_B = 8314.457947; 

// Biot Savart constant
// constexpr float K_BS = 1e-7;
constexpr float K_BS = 0;

// Mass of a proton
constexpr float M_PROTON = 1.67262192369e-27;

// Mass of a neutron
constexpr float M_NEUTRON = 1.67492749804e-27;

// Mass of an electron
constexpr float M_ELECTRON = 9.1093837015e-31;

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
    float mass;
    float charge;
    float percentage;

    InsertedParticle() : alias(""), mass(0), charge(0), percentage(0) {}
    InsertedParticle(std::string alias, float mass, float charge, float percentage) : alias(alias), mass(mass), charge(charge), percentage(percentage) {}
};

struct Point
{
    float x;
    float y;
    float z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct CSVPoint
{
    std::string alias;
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
    float mass;
    float charge;

    CSVPoint() : alias(""), x(-1), y(-1), z(-1), vx(-1), vy(-1), vz(-1), mass(-1), charge(-1) {}
    CSVPoint(std::string alias, float x, float y, float z, float vx, float vy, float vz, float mass, float charge) : alias(alias),
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

    float forceX = 0, forceY = 0, forceZ = 0;

    float mass = 0;
    float velocityX = 0, velocityY = 0, velocityZ = 0; // current velocity of the particle
    std::vector<Octree *> children;

    // Change values accordingly
    float magneticFieldX = 0;
    float magneticFieldY = 0;
    float magneticFieldZ = 0;

    float electricFieldX = 0;
    float electricFieldY = 0;
    float electricFieldZ = 0;
    // -------------------------

    float charge = 0;         // @attention IMPORTANT: USE THIS VALUE when calculating charge of single particles!
    float positiveCharge = 0; // @attention IMPORTANT: positiveCharge and negativeCharge do not exist on single particles, and only for octrees! DO NOT USE for single particles
    float negativeCharge = 0; // @attention IMPORTANT: positiveCharge and negativeCharge do not exist on single particles, and only for octrees! DO NOT USE for single particles

    std::string alias;

    Octree();
    Octree(std::string alias, float x, float y, float z, float vx, float vy, float vz, float mass, float charge);
    Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    void recalculateCenterOfCharge(Octree *octree);

    void insert(Octree *root, std::string alias, float x, float y, float z, float vx, float vy, float vz, float mass, float charge);
    bool find(float x, float y, float z);
};

class Barnes
{
public:
    bool isExternalNode(Octree *octree);
    void calcForce(Octree *node, Octree *b, float thetaLimit);
    void addForce(Octree *node, Octree *b, float posdx, float posdy, float posdz, float negdx, float negdy, float negdz);
};

class Simulation
{
private:
public:
    void mainLoop(Octree *&volume, float timeStep);
};

std::vector<CSVPoint> loadInitialValues();
std::vector<CSVPoint> generateInitialPoints(Octree *&octree,
                                            float density,
                                            float temperature,
                                            std::vector<InsertedParticle> particles,
                                            Shape shape,
                                            std::initializer_list<float> dimensions, 
                                            bool isLiner = false,
                                            bool append = false,
                                            bool load = true);

std::vector<Octree *> getChildren(Octree *volume);

void clearSimulationValues();
void writeSimulationValues(std::vector<Octree *> children, std::ofstream &ValueFile);

int tester();