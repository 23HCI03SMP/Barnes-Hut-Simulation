#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Path to initial conditions csv file
constexpr char INITIAL_VALUES_PATH[] = "initial_values.csv";

// Path to simulation results csv file
constexpr char SIMULATION_VALUES_PATH[] = "simulation_values.csv";

// Path to theta csv file
constexpr char THETA_PATH[] = "theta.csv";

// Pi constant
constexpr float PI = 3.14159265358979323846;

// Coulomb's constant
constexpr float K_E = 8.9875517923e9;

// Boltzmann constant
constexpr float K_B = 1.380649e-23;

// Mass of a proton
constexpr float M_PROTON = 1.67262192369e-27;

// Mass of a neutron
constexpr float M_NEUTRON = 1.67492749804e-27;

// Mass of an electron
constexpr float M_ELECTRON = 9.1093837015e-31;

struct Point
{
    float x;
    float y;
    float z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct Particle
{
    std::string alias;
    float mass;
    float charge;
    float percentage;

    Particle() : alias(""), mass(-1), charge(-1), percentage(-1) {}
    Particle(std::string alias, float mass, float charge, float percentage) : alias(alias), mass(mass), charge(charge), percentage(percentage) {}
};

struct CSVPoint
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
    float mass;
    float charge;
    std::string alias;
    CSVPoint() : x(-1), y(-1), z(-1), vx(-1), vy(-1), vz(-1), mass(-1), charge(-1), alias("") {}
    CSVPoint(float x,
             float y,
             float z,
             float vx,
             float vy,
             float vz,
             float mass,
             float charge,
             std::string alias) : x(x),
                                  y(y),
                                  z(z),
                                  vx(vx),
                                  vy(vy),
                                  vz(vz),
                                  mass(mass),
                                  charge(charge),
                                  alias(alias) {}
};

class Octree
{
private:
public:
    Point *point;
    Point *minPoints, *maxPoints;
    Point *com;

    std::string alias;
    float forceX = 0;
    float forceY = 0;
    float forceZ = 0;
    float charge = 0;
    float mass = 0;
    float velocityX = 0, velocityY = 0, velocityZ = 0;
    std::vector<Octree *> children;

    Octree();
    Octree(float x, float y, float z, float vx, float vy, float vz, float mass, float charge, std::string alias);
    Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    void insert(Octree *root, float x, float y, float z, float vx, float vy, float vz, float mass, float charge, std::string alias);
    bool find(float x, float y, float z);

    void recalculateCenterOfMass(Octree *octree);
};

class Barnes
{
private:
    bool isExternalNode(Octree *octree);

public:
    void calcForce(Octree *node, Octree *b, float thetaLimit);
};

class Simulation
{
private:
public:
    Octree mainLoop(Octree *volume, int iterations, float timeStep);
};

std::vector<CSVPoint> loadInitialValues();
void loadAndInsertInitialValues(Octree *octree);
std::vector<CSVPoint> generateInitialPoints(float minX, float minY, float minZ,
                                            float maxX, float maxY, float maxZ,
                                            float radius,
                                            float density,
                                            float temperature,
                                            std::vector<Particle> particles);
std::vector<CSVPoint> generateInitialPoints(float minX, float minY, float minZ,
                                            float maxX, float maxY, float maxZ,
                                            float radius,
                                            int particleNumber,
                                            float temperature,
                                            std::vector<Particle> particles);

std::vector<Octree *> getChildren(Octree *volume);
std::vector<Octree *> getNodes(Octree *volume);

void generateFiles(std::vector<CSVPoint> points);
void initialiseSimulationValuesFile(std::vector<CSVPoint> initialPoints);
void generateSimulationValuesFile(Octree *octree);