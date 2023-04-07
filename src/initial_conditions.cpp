#include "include/octree.h"
#include <vector>
#include <gsl/gsl_rng.h>

// Returns d-dimentional random unit vector (norm = 1)
std::vector<> randUnitVector(int d, gsl_rng *r) {
    float[] phi = gsl_rng_uniform(r);
}

/*
Returns the positions and velocities of particles in the Plummer sphere

input:
    Npart - total number of particles to be initialized
    a - Plummer radius (scale parameter setting the cluster core)
    m - mass of particles (if single value, all particles will be
            initialized with the same mass; if array, each particles will
            have mass m_i)
    G - Gravitational constant; by default, work in Kpc-GM_sun-Myr units
    seed - random generator seed (int, optional: defaults to numpy seed)

output:
    [pos, vel] - [(Npart x 3), (Npart x 3)] of positions and velocities
                    in cartesian coordinates
*/
std::vector<> plummer(int Npart, float a, float m=1, float G=4.483e-3, gsl_rng *r) {

}