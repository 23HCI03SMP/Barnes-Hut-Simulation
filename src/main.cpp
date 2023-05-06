#include "include/barnesHut.h"
#include <iostream>
#include <chrono>
#include <gsl/gsl_randist.h>

using namespace std::chrono;

// Octree loop(Octree octreeM, int iterations, float theta, float timeStep)
// {
//     Octree *octree = &octreeM;

//     Octree final = Octree(0, 0, 0, 0, 0, 0);

//     for (int i = 0; i < iterations; i++)
//     {
//         std::vector<Octree *> childVect = getNodes(octree);
//         Barnes barnes;

//         for (Octree *&child : childVect)
//         {
//             barnes.calcForce(octree, child, theta);
//         }

//         Simulation sim = Simulation();
//         final = sim.mainLoop(octree, 1, timeStep);

//         generateSimulationValuesFile(&final);
//     }

//     return final;
// }

int main()
{
    Octree tree(1, 1, 1, 5, 5, 5);

    const int N = 5000;            // Number of coordinates
    const double lower_bound = 1.0; // Lower bound for coordinates
    const double upper_bound = 5.0; // Upper bound for coordinates

    // Initialize random number generator
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_default);

    // Seed the random number generator with the current time
    gsl_rng_set(rng, time(NULL));

    // Generate random coordinates within the given range
    for (int i = 0; i < N; i++)
    {
        float x = gsl_ran_flat(rng, lower_bound, upper_bound); // x coordinate
        float y = gsl_ran_flat(rng, lower_bound, upper_bound); // y coordinate
        float z = gsl_ran_flat(rng, lower_bound, upper_bound); // z coordinate
    
        tree.insert(x, y, z);
        std::cout << i << "\n";
    }
    
    return 0;
}