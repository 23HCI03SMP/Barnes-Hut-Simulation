#include "include/octree.h"
#include <array>

class node{
    //min and max points (2x2x2 box)
    float box[3][2] = {
        {-1, 1},
        {-1, 1},
        {-1, 1}
    };
    float xLow = box[0][0];
    float xMax = box[0][1];
    float yLow = box[1][0];
    float yMax = box[1][1];
    float zLow = box[2][0];
    float zMax = box[2][1];
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

    //create child nodes
    void createChildren(node node, bbox box){
        float xhalf = centre[0];
        float yhalf = centre[1];
        float zhalf = centre[2];
        //create octets
        bbox c1;
        c1.box =  {
            {xLow, xhalf},
            {yLow, yhalf},
            {zLow, zhalf}
        };
        bbox c2;
        c2.box =  {
            {xhalf, xMax},
            {yLow, yhalf},
            {zLow, zhalf}
        };
        bbox c3;
        c3.box =  {
            {xLow, xhalf},
            {yhalf, yMax},
            {zLow, zhalf}
        };
        bbox c4;
        c4.box =  {
            {xhalf, xMax},
            {yhalf, yMax},
            {zLow, zhalf}
        };
        bbox c5;
        c5.box =  {
            {xLow, xhalf},
            {yLow, yhalf},
            {zhalf, zMax}
        };
        bbox c6;
        c6.box =  {
            {xhalf, xMax},
            {yLow, yhalf},
            {zhalf, zMax}
        };
        bbox c7;
        c7.box =  {
            {xLow, xhalf},
            {yhalf, yMax},
            {zhalf, zMax}
        };
        bbox c8;
        c8.box =  {
            {xhalf, xMax},
            {yhalf, yMax},
            {zhalf, zMax}
        };

        children.push_back(c1);
        children.push_back(c2);
        children.push_back(c3);
        children.push_back(c4);
        children.push_back(c5);
        children.push_back(c6);
        children.push_back(c7);
        children.push_back(c8);
    };
};
