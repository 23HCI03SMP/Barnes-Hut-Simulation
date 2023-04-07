#include <list>
#include <array>
#include <vector>

class Particle{
    public:
    float position[3]= {};
};

class bbox{
    public:
    float xLow = box[0][0];
    float xMax = box[0][1];
    float yLow = box[1][0];
    float yMax = box[1][1];
    float zLow = box[2][0];
    float zMax = box[2][1];
    float centre[3] = {(xLow + xMax)/2, (yLow, yMax)/2, (zLow, zMax)/2};
    std::vector<bbox> children;
    //number of particles
    int n = 0;
    Particle particle;
    //position of centre of mass
    Particle com;
    std::vector<std::vector<float>> box;
};