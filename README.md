# Barnes Hut Model for Charged Particles

## Description
The Barnes Hut Model is a method for calculating the gravitational force between charged particles. It is a tree-based algorithm that uses a quadtree or octree to calculate the force between particles. The algorithm is described in the paper [A hierarchical O(N log N) force-calculation algorithm](https://www.doi.org/10.1038/324446a0) by Barnes and Hut.

## Prerequisites
- Visual Studio Code
  - C/C++ Extension
  - Nsight Visual Studio Code Edition Extension
  - [Nsight Visual Studio Code Edition](https://developer.nvidia.com/nsight-visual-studio-code-edition)
- C++
  - GSL (remember to compile with `-lgsl -lgslcblas -lm` flags)
- Python 3
  - matplotlib
  - pandas
  - numpy

## To-do list

- [x] octree.cpp - Octree data structure
- [x] octree.cpp - Update Center of Mass/Charge
- [x] barnes.cpp - Calculate Force
- [x] simulation_loop.cpp - Calculate Acceleration
- [x] simulation_loop.cpp - Update tree and particle position
- [x] initialConditions.cpp
- [x] initialConditions.cpp - implement initial velocity
- [x] Create initial values csv file
- [ ] Utilise OpenMP/CUDA to parallelise the code
- [ ] Different types of particles
- [x] Optimise Insertion
- [ ] Check Energy Conservation
- [x] Implement Magnetic Field

## Running the project
### Visualising generated points
Run `plotter.py` to view `initial_values.csv` generated by `initial_conditions.cpp`. The plotter uses matplotlib to plot the points in 3D space.

## Methodology

### Octree
The octree is a tree data structure that is used to partition space. Each node in the tree represents a cube in space. The root node represents the entire space. Each node has 8 children, which represent the 8 cubes that make up the parent cube. 

![Octree with numberings used in code](images/octet.png)

The children of a node are only created if the node contains more than one particle. The particles are then distributed among the children. The algorithm for creating the octree is as follows:

1. Create the root node, which represents the entire space
2. When a new particle is added to the tree, check if there is a particle in the root node
   1. If there is no particle in the root node, add the particle to the root node
   2. If there is a particle in the root node, create 8 children for the root node (i.e split the root node into 8 3d octets) and move the particles to the appropriate children
3. Repeat step 2 until all particles are in the tree

### Coulomb's Law

The octree allows for us to calculate the force between particles in O(N log N) time. For the calculation of the force between charged particles, we can use Coulomb's law:
$$F = \frac{kq_1q_2}{r^2}$$
where F is the electric force, $k$ is the Coulomb constant, $q_1$ and $q_2$ are the charges of the particles, and $r$ is the distance between the particles. The force between particles can be calculated by summing the forces between each particle and all other particles. This is an O(N^2) operation.

### Biot-Savart Law

Using the Biot-Savart Law, we can derive another equation to [calculate the magnetic forces between moving charges](https://www.phys.unsw.edu.au/einsteinlight/jw/module2_FEB.htm):
$$F=\frac{\mu_{0}}{4\pi}\frac{q_{1}q_{2}\overrightarrow{v_{1}}\times \overrightarrow{v_{2}} \times \overrightarrow{r}}{\left| {r}^{3} \right|}
where F is the magnetic force, $$\mu_{0} is the permitivity of free space, r is the distance between the two particles, q represents the charge and v represents the velocity where subscripts 1 and 2 represent the two interacting particles.

### Centre of Charge

By finding the centre of charge of each octet, we are able to treat all particles as a single particle at the point of the centre of charge. By splitting the centre of charge into its positive and negative components, (i.e different centre of charges for positive and negative charge), we are able to subtract the different values of positive and negative charge to obtain the correct resultant force. This allows us to simulate multiple particle types with different charges at the same time.

### Initial Values
The initial values for the particles are generated using the `initial_conditions.cpp` file. The particles are generated in a sphere within the given boundaries in the octree. The particles are given a certain charge and mass (based on what particle we are simulating), as well as a certain coordinate distrubution using Gaussian Distribution:

$$f(x) = \frac{1}{\sigma\sqrt{2\pi}}e^{-\frac{1}{2}(\frac{x-\mu}{\sigma})^2}$$

where $\mu$ is the mean and $\sigma$ is the standard deviation. The particles are given a random position within the sphere, and a random velocity. The velocity is given a random direction, and a random magnitude. The magnitude of the velocity is calculated using the following equation:

The number of particles is given by the following equation:

$$N = \lceil \frac{4}{3}\pi r^3 \rho \rceil$$

where $r$ is the radius of the sphere, and $\rho$ is the density of the sphere. 

Their initial velocities in the (x, y, z) directions are calculated using the Maxwell-Boltzmann distribution:

$$v = (\frac{kT}{m})^\frac{1}{2}$$

where k is the Boltzmann constant, T is the temperature, and m is the mass of the particle.

### Barnes Hut Simulation
While summing the forces between every particle is an O(N^2) operation, the Barnes Hut Model uses the octree to reduce the number of particles that need to be considered when calculating the force between particles. [The algorithm](http://arborjs.org/docs/barnes-hut) for calculating the force between particles is as follows:
1. If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
2. Otherwise, calculate the ratio s/d, where s is the width of the region represented by the internal node, and d is the distance between the body and the node’s center-of-mass. 
3. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
4. Otherwise, run the procedure recursively on each of the current node’s children.

The Barnes Hut Model is an O(N log N) algorithm. The value of θ is a parameter that affects the degree of approximation. The smaller the value of θ, the more accurate the simulation is. However, the smaller the value of θ, the longer the simulation takes to run.
