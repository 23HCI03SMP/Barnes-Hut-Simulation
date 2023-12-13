#include "include/barnesHut.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

using namespace std::chrono;

void loop(Octree *octree, int iterations, double theta, double timeStep)
{
    std::cout << "Starting simulation loop of theta of " << theta << " and time step of " << timeStep << " for " << iterations << " iterations" << std::endl;

    Octree *final = octree;
    std::ofstream ValueFile(std::filesystem::current_path() / SIMULATION_VALUES_PATH, std::ios::app);

    double totalDur = 0;
    double totalProcessingDur = 0;
    double EpeSum = 0;
    double BpeSum = 0;

    // @note: Basically there's a change in how values are logged to the simulation_values.csv. We no longer
    //        log the values of the initial conditions, but rather the values of the simulation before each loop is performed.
    //        That way we avoid needing to run getChildren() multiple times
    for (int i = 0; i <= iterations; i++)
    {
        // Start timer
        auto start = high_resolution_clock::now();

        // Get all particles in octree
        std::vector<Octree *> children = getChildren(final);
        auto t1 = high_resolution_clock::now(); // Time taken to get children
        writeSimulationValues(children, ValueFile);
        auto t2 = high_resolution_clock::now(); // Time taken to write values to file

        Barnes barnes;
        for (Octree *child : children)
        {
            // calculate and update forces/fields
            barnes.calcForce(final, child, theta);
        }

        // for (Octree *i:children)
        // {
        //     for (Octree *j:children)
        //     {
        //         barnes.calcForce(i, j, theta);
        //     }
        // }

        //calculate pe between children in a list of chidren
        //lop through children and calculate pe between each pair
        //add pe to total pe
        // for (int i = 0; i < children.size(); i++)
        // {
        //     for (int j = i + 1; j < children.size(); j++)
        //     {
        //         Octree *node = children[i];
        //         Octree *b = children[j];

        //         double distance = sqrt(pow(node->point->x - b->point->x, 2) + pow(node->point->y - b->point->y, 2) + pow(node->point->z - b->point->z, 2));
        //         double pe = (K_E * node->charge * b->charge) / distance; 

        //         EpeSum += pe;
        //     }
        // }

        // Update positions and velocities
        Simulation sim = Simulation();
        sim.mainLoop(final, timeStep);

        // Stop timer
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        auto processingDuration = duration_cast<milliseconds>(stop - t2) + duration_cast<milliseconds>(t1 - start);

        totalDur += duration.count();
        totalProcessingDur += processingDuration.count();

        std::cout << "Completed iteration " << i << " in " << duration.count() << "ms (" << processingDuration.count() << "ms processing)" << std::endl;

        // output pe to pe.txt
        std::ofstream peFile;
        peFile.open(std::filesystem::current_path() / "pe.txt", std::ios_base::app);
        peFile << EpeSum << std::endl;

        EpeSum = 0;
    }

    ValueFile.close();
    std::cout << "Average time: " << totalDur / iterations << "ms (" << totalProcessingDur / iterations << "ms processing)" << std::endl;
}

int main()
{
    // Define particles in plasma
    std::vector<InsertedParticle> fuel_particles = {
        InsertedParticle("Deuteron", 2, 1, 0.5),
        InsertedParticle("Electron", 1 / 1836.0f, -1, 0.5),
    };

    std::vector<InsertedParticle> test_particles = {
        InsertedParticle("Deuteron", 2 * 1836, 1, 0.5),
        InsertedParticle("Electron", 1, -1, 0.5),
    };

    std::vector<InsertedParticle> test_particles_2 = {
        InsertedParticle("Deuteron", M_NEUTRON + M_PROTON, 1, 0.5),
        InsertedParticle("Electron", M_ELECTRON, -1, 0.5),
    };

    std::vector<InsertedParticle> liner_particles = {
        InsertedParticle("Beryllium", 9, 2, 0.333),
        InsertedParticle("LinerElectron", 1 / 1836.0f, -1, 0.667)
    };

    std::filesystem::path filePath = std::filesystem::current_path() / "energy.txt";
    std::remove(filePath.string().c_str());

    // Create Octree object
    Octree tree = Octree(0, 0, 0, 500, 500, 500);
    Octree *tree_ptr = &tree;

    // generateInitialPoints(tree_ptr, 50, 34800, fuel_particles, Shape::REGULAR_CYLINDER, {2, 10});               // Generate hot rod
    // generateInitialPoints(tree_ptr, 100, 293, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true, true); // Generate liner
    // generateInitialPoints(tree_ptr, 5, 294, liner_particles, Shape::HOLLOW_CYLINDER, {2, 4, 10}, true);
    // generateInitialPoints(tree_ptr, 100, 11000, fuel_particles, Shape::SPHERE, {4});
    // generateInitialPoints(tree_ptr, 100, 293, {InsertedParticle("Deuteron", 2, 1, 0.5)}, Shape::SPHERE, {4}); // Generate liner
    // generateInitialPoints(tree_ptr, 0.00125, 11.6e6, test_particles_2, Shape::SPHERE, {130}); 
    auto a = loadInitialValues();

    for (CSVPoint point : a)
    {
        tree_ptr->insert(tree_ptr, new Octree(point.alias, point.x, point.y, point.z, point.vx, point.vy, point.vz, point.mass, point.charge));
    }

    // tree_ptr->insert(tree_ptr, new Octree("Electron", 1, 1, 1, 0, 0, 0, 1, -1));
    // tree_ptr->insert(tree_ptr, new Octree("Electron", 10, 10, 10, 0, 0, 0, 1, -1));

    // Start simulation loop
    loop(tree_ptr, 5, 0.5, 1e-8);

    // read from python_run.txt to get python run command and then run animator.py using console commands
    std::ifstream pyRunFile;
    pyRunFile.open(std::filesystem::current_path() / "python_run.txt");
    std::string pyRun;
    std::getline(pyRunFile, pyRun);
    pyRunFile.close();
    std::string cmd = pyRun + " ./animator.py";

    // Animate and generate .vtk files
    system(cmd.c_str());
    // system("py ./vtk_animator.py");

    return 0;
}
