#include "include/barnesHut.h"
#include <iostream>
#include <chrono>

#include "testing.cpp"

using namespace std::chrono;

void loop(Octree *octree, int iterations, float theta, float timeStep)
{
    Octree *final = octree;

    float totalDur = 0;

    for (int i = 0; i < iterations; i++)
    {
        auto start = high_resolution_clock::now();
        std::vector<Octree *> childVect = getChildren(final);
        Barnes barnes;

        for (Octree *child : childVect)
        {
            barnes.calcForce(final, child, theta);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        totalDur += duration.count();
        std::cout << duration.count() << " ms " << std::endl;

        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        generateSimulationValuesFile(final);
    }

    std::cout << "Average time: " << totalDur / iterations << " ms" << std::endl;
}

int main()
{
    std::vector<InsertedParticle> particles = {
        InsertedParticle("Deutron", 2, 1, 0.5),
        InsertedParticle("Electron", 1/1823.0f, 1, 0.5),
    };

    std::cout << "Starting simulation..." << std::endl;
    // std::vector<CSVPoint> points = generateInitialPoints(1, 1, 1, 5, 5, 5, 2, 2, 2, 1, 100, 293, particles, Shape::SPHERE); // 293K = 20C
    // generateInitialValuesFile(points);

    // std::vector<CSVPoint> initialPoints = loadInitialValues();

    Octree tree = Octree(1, 1, 1, 5, 5, 5);
    Octree *tree_ptr = &tree;

    std::vector<CSVPoint> points = generateInitialPoints(tree_ptr, 1, 1, 1, 5, 5, 5, 2, 2, 2, 100, 294, particles, Shape::SPHERE); // 293K = 20C

    loop(tree_ptr, 200, 0, 1e-10);

    std::cout << "\nAnimator Starting...\n";
    system("py ./animator.py");

    return 0;
}