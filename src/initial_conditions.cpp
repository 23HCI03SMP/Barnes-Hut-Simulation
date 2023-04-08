#include <array>
#include <numeric>
#include "include/octree.h"
#include <vector>
#include <gsl/gsl_rng.h>
#include <cmath>

// Returns d-dimentional random unit vector (norm = 1)
std::vector<double> randUnitVector(int d, gsl_rng *r) {
    /*
    Since gsl_rng_uniform only returns values in [0,1), we need to use the equation 

    lower_bound + (upper_bound - lower_bound) * gsl_rng_uniform() 
    */
    double phi = pi() * 2 * gsl_rng_uniform(r); // rand.uniform(0, np.pi*2)
    double costheta = 2 * gsl_rng_uniform(r) - 1; // rand.uniform(-1, 1)
    double theta = std::acos(costheta); // np.arccos(costheta)

    double x = std::sin(theta) * std::cos(phi);
    double y = std::sin(theta) * std::sin(phi);
    double z = std::cos(theta);

    std::vector<double> unitVector = {x, y, z};
    return unitVector;
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
std::vector<std::array<int, 3>> plummer(int Npart, float a, gsl_rng *r, float m=1, float G=4.483e-3) {
    std::vector<int> pos = plummerDist_3d_xyz(Npart, a, r);
    int M;

    if (pos.size() == 1) {
        M = Npart * m;
    } else {
        M = std::accumulate(pos.begin(), pos.end(), 0);
    }
}

/*
Initializes particles in 3d with Plummer density profile.
input:
    Npart - total number of particles to be initialized
    a - Plummer radius (scale parameter setting the cluster core)
    rand - RandomState generator (optional: defaults to numpy.random)
output:
    pos - (Npart x 3) array of positions in cartesian coordinates
*/
std::vector<int> plummerDist_3d_xyz(int Npart, auto a, gsl_rng *r) {
    std::vector<double> r(Npart); 
    std::vector<double> pos(Npart, 3);

    for (int i = 0; i < Npart; i++) {
        // Let enclosed mass function f_mi be random number between 0 and 1
        double f_mi = gsl_rng_uniform(r);
        r[i] = a/std::sqrt(std::pow(f_mi, -2.0/3.0) - 1.0);


        pos[i] = r[i] * randUnitVector(3, r);
    }

    return pos;
}