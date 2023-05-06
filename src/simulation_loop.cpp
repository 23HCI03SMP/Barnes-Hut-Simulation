// #include <iostream>
// #include "include/barnesHut.h"

// std::vector<Octree *> getChildren(Octree *&volume)
// {
//     std::vector<Octree *> childrenList;
//     for (Octree *child : volume->children)
//     {
//         if (child->children.size() == 0 && child->point != nullptr && child->point->x != -1)
//         {
//             childrenList.push_back(child);
//         }
//         std::vector<Octree *> recursChildren = getChildren(child);
//         childrenList.insert(childrenList.end(), recursChildren.begin(), recursChildren.end());
//     }
//     return childrenList;
// }

// std::vector<Octree *> getNodes(Octree *&volume)
// {
//     std::vector<Octree *> childrenList;
//     for (Octree *child : volume->children)
//     {
//         if (child->mass != 0)
//         {
//             childrenList.push_back(child);
//         }
//         std::vector<Octree *> recursChildren = getChildren(child);
//         childrenList.insert(childrenList.end(), recursChildren.begin(), recursChildren.end());
//     }
//     return childrenList;
// }

// Octree Simulation::mainLoop(Octree *&volume, int iterations, float timeStep)
// { // simulation volume + iterations (-1 for infinite iterations) + time step (i.e how many seconds are in each iteration)
//     // simulation loop
//     Octree newOctree = Octree(
//         volume->minPoints->x,
//         volume->minPoints->y,
//         volume->minPoints->z,
//         volume->maxPoints->x,
//         volume->maxPoints->y,
//         volume->maxPoints->z);

//     Octree *newOctreePtr = &newOctree;

//     for (int i = 0; i < iterations; i++)
//     {
//         std::vector<Octree *> childrenList = getChildren(volume);
//         for (Octree *child : childrenList)
//         {
//             // update accelerations
//             float accelX = child->forceX / child->mass;
//             float accelY = child->forceY / child->mass;
//             float accelZ = child->forceZ / child->mass;

//             // calculate displacement using suvat
//             float sX = child->velocityX * timeStep + (0.5 * accelX * timeStep * timeStep);
//             float sY = child->velocityY * timeStep + (0.5 * accelY * timeStep * timeStep);
//             float sZ = child->velocityZ * timeStep + (0.5 * accelZ * timeStep * timeStep);

//             child->point->x += sX;
//             child->point->y += sY;
//             child->point->z += sZ;

//             // update simulation volume
//             newOctree.insert(
//                 newOctreePtr,
//                 child->point->x,
//                 child->point->y,
//                 child->point->z,
//                 child->velocityX,
//                 child->velocityY,
//                 child->velocityZ,
//                 child->mass,
//                 child->charge);
//         }
//     }
//     return newOctree;
// }