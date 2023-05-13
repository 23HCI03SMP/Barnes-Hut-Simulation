#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

Octree loop(Octree* octree, int iterations, float theta, float timeStep)
{
    Octree final = Octree(0, 0, 0, 0, 0, 0);

    for (int i = 0; i < iterations; i++)
    {
        std::vector<Octree *> childVect = getNodes(octree);
        Barnes barnes;

        for (Octree *child : childVect)
        {
            barnes.calcForce(octree, child, theta);
        }

        Simulation sim = Simulation();
        final = sim.mainLoop(octree, 1, timeStep);

        generateSimulationValuesFile(&final);
        std::cout << "Timestep Finished\n";
    }

    return final;
}

int main()
{
    std::vector<Particle> particles = {
        Particle("Deutron", M_PROTON + M_NEUTRON, 1, 0.5),
        Particle("Electron", M_ELECTRON, -1, 0.5),
    };

    auto start = high_resolution_clock::now();
    std::vector<CSVPoint> points = generateInitialPoints(1.0f, 1.0f, 1.0f, 10.0f, 10.0, 10.0f, 4.0f, 20.0f, 293.0f, particles); // 293K = 20C
    generateFiles(points);

    std::cout << "Timer Started\n";

    Octree tree = Octree(1, 1, 1, 20, 20, 20);
    Octree *tree_ptr = &tree;

    loadAndInsertInitialValues(tree_ptr);
    std::cout << "Initial Values Loaded\n";

    std::cout << "Initial Values File Created\n";

    Octree final = loop(tree_ptr, 10, 1, 1e-10); 

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << " ms " << duration.count()/1000 << " s" << std::endl;

    // std::getchar();
    return 0;
}