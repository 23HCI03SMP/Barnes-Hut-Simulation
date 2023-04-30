#include "include/barnesHut.h"

void calcForce(Octree *&node, Octree *&b)
{
    float dx = node->com->x - b->com->x;
    float dy = node->com->y - b->com->y;
    float dz = node->com->z - b->com->z;

    float length = abs(node->minPoints->x - node->maxPoints->x);
    float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    float theta = length/distance;
    float thetaLim = 0.5;

    if (theta < thetaLim)
    {
        //do stuff
    }
    else
    {
        for (Octree *child : node->children)
        {
            calcForce(child, b);
        }
    }
}