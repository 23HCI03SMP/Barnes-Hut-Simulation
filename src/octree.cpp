#include "include/octree.h"
#include <array>

class node{
    //min and max points (2x2x2 box)
    float box[3][2] = {
        {-1, 1},
        {-1, 1},
        {-1, 1}
    };
    int xLow = box[0][0];
    int xMax = box[0][1];
    int yLow = box[1][0];
    int yMax = box[1][1];
    int zLow = box[2][0];
    int zMax = box[2][1];
    float centre[3] = {(xLow + xMax)/2, (yLow, yMax)/2, (zLow, zMax)/2};
    std::list<bbox> children;
    //number of particles
    int n = 0;
    Particle particle;
    //position of centre of mass
    Particle com;


    template<class C, typename T>
    bool contains(C&& c, T t) {
        return std::find(std::begin(c), std::end(c), t) != std::end(c);
    };

    void insertParticle(Particle particle, bbox box)
    {
        bool inBox;

        //Check if particle already exists (at location)
        if (particle.position[0] < xLow || particle.position[0] > xMax){
            if (particle.position[1] < yLow || particle.position[1] > yMax){
                if (particle.position[2] < zLow || particle.position[2] > zMax){
                    return;
                }
            }
        }

        if (n == 0){
            particle = particle;
            com = particle; //set centre of mass to particle position

        }
        else{
            if (n == 1){
                
            }
        }
        
        n++;
    }

    void createChildren(node node, bbox box){
        float xhalf = centre[0];
        float yhalf = centre[1];
        float zhalf = centre[2];
        //create octets
        float c1[3][2] = {
            {xLow, xhalf},
            {yLow, yhalf},
            {zLow, zhalf}
        };
    };
};
