#include "include/octree.h"
#include <array>

class node{
    //min and max points (2x2x2 box)
    bbox box;

    box.box = {
        {-1, 1},
        {-1, 1},
        {-1, 1}
    };

    void insertParticle(Particle particle, bbox box)
    {
        bool inBox;

        //Check if particle already exists (at location)
        if (particle.position[0] < box.xLow || particle.position[0] > box.xMax){
            if (particle.position[1] < box.yLow || particle.position[1] > box.yMax){
                if (particle.position[2] < box.zLow || particle.position[2] > box.zMax){
                    return;
                }
            }
        }

        //if there are no particles in the node or its children, must be external node
        if (box.n == 0){
            box.particle = particle;
            box.com = particle; //set centre of mass to particle position

        }
        //else internal node
        else{
            if (box.n == 1){
                createChildren(box);
                for (int i = 0; i < size(box.children); i++){
                    insertParticle(box.particle, box.children[i]); //insert original particle into each box and check if particle exists in box
                }
                box.particle = Particle();
            }
            for (int i = 0; i < size(box.children); i++){
                insertParticle(particle, box.children[i]); //insert new particle into each box and check if particle exists in box
            }
        }
        
        updateCom(box); // update centre of mass
        box.n++;
    }

    //create child nodes
    void createChildren(bbox box){
        float xhalf = box.centre[0];
        float yhalf = box.centre[1];
        float zhalf = box.centre[2];
        //create octets
        bbox c1;
        c1.box =  {
            {box.xLow, xhalf},
            {box.yLow, yhalf},
            {box.zLow, zhalf}
        };
        bbox c2;
        c2.box =  {
            {xhalf, box.xMax},
            {box.yLow, yhalf},
            {box.zLow, zhalf}
        };
        bbox c3;
        c3.box =  {
            {box.xLow, xhalf},
            {yhalf, box.yMax},
            {box.zLow, zhalf}
        };
        bbox c4;
        c4.box =  {
            {xhalf, box.xMax},
            {yhalf, box.yMax},
            {box.zLow, zhalf}
        };
        bbox c5;
        c5.box =  {
            {box.xLow, xhalf},
            {box.yLow, yhalf},
            {zhalf, box.zMax}
        };
        bbox c6;
        c6.box =  {
            {xhalf, box.xMax},
            {box.yLow, yhalf},
            {zhalf, box.zMax}
        };
        bbox c7;
        c7.box =  {
            {box.xLow, xhalf},
            {yhalf, box.yMax},
            {zhalf, box.zMax}
        };
        bbox c8;
        c8.box =  {
            {xhalf, box.xMax},
            {yhalf, box.yMax},
            {zhalf, box.zMax}
        };

        box.children.push_back(c1);
        box.children.push_back(c2);
        box.children.push_back(c3);
        box.children.push_back(c4);
        box.children.push_back(c5);
        box.children.push_back(c6);
        box.children.push_back(c7);
        box.children.push_back(c8);
    };

    void updateCom(bbox box){
        box.com = {0, 0 , 0};
        //Assume all have charge 1 *Instead of using multipole expansion we will use centre of mass for now
        //since there is only one particle in each node, the centre of gravity for that node will be the particle position
        box.com = box.particle;
    }   
};
