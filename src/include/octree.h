#include <list>
#include <array>
#include <vector>

class Particle{
    public:
    float position[3]= {};
};

class bbox{
    public:
    std::vector<std::vector<float>> box;
};