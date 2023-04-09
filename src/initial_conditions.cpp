#include <array>
#include <numeric>
#include "include/barnesHut.h"
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_blas.h>
#include <cmath>



// // Returns d-dimentional random unit vector (norm = 1)
// std::vector<double> randUnitVector(int d, gsl_rng *r) {
//     /*
//     Since gsl_rng_uniform only returns values in [0,1), we need to use the equation 

//     lower_bound + (upper_bound - lower_bound) * gsl_rng_uniform() 
//     */
//     double phi = pi() * 2 * gsl_rng_uniform(r); // rand.uniform(0, np.pi*2)
//     double costheta = 2 * gsl_rng_uniform(r) - 1; // rand.uniform(-1, 1)
//     double theta = std::acos(costheta); // np.arccos(costheta)

//     double x = std::sin(theta) * std::cos(phi);
//     double y = std::sin(theta) * std::sin(phi);
//     double z = std::cos(theta);

//     std::vector<double> unitVector = {x, y, z};
//     return unitVector;
// }

// /*
// Returns the positions and velocities of particles in the Plummer sphere

// input:
//     Npart - total number of particles to be initialized
//     a - Plummer radius (scale parameter setting the cluster core)
//     m - mass of particles (if single value, all particles will be
//             initialized with the same mass; if array, each particles will
//             have mass m_i)
//     G - Gravitational constant; by default, work in Kpc-GM_sun-Myr units
//     seed - random generator seed (int, optional: defaults to numpy seed)

// output:
//     [pos, vel] - [(Npart x 3), (Npart x 3)] of positions and velocities
//                     in cartesian coordinates
// */
// std::vector<std::array<int, 3>> plummer(int Npart, float a, gsl_rng *r, float m=1, float G=4.483e-3) {
//     std::vector<int> pos = plummerDist_3d_xyz(Npart, a, r);
//     int M;

//     if (pos.size() == 1) {
//         M = Npart * m; // If all particles have the same mass
//     } else {
//         M = std::accumulate(pos.begin(), pos.end(), 0);
//     }


// }

// /*
// Initializes particles in 3d with Plummer density profile.
// input:
//     Npart - total number of particles to be initialized
//     a - Plummer radius (scale parameter setting the cluster core)
//     rand - RandomState generator (optional: defaults to numpy.random)
// output:
//     pos - (Npart x 3) array of positions in cartesian coordinates
// */
// std::vector<int> plummerDist_3d_xyz(int Npart, double a, gsl_rng *r) {
//     std::vector<double> r(Npart); 
//     std::vector<double> pos(Npart, 3);

//     for (int i = 0; i < Npart; i++) {
//         // Let enclosed mass function f_mi be random number between 0 and 1
//         double f_mi = gsl_rng_uniform(r);
//         r[i] = a/std::sqrt(std::pow(f_mi, -2.0/3.0) - 1.0);


//         pos[i] = r[i] * randUnitVector(3, r);
//     }

//     return pos;
// }

// /*
// Returns velocities from the velocity distribution of particles
// at radius r in the Plummer model

// input:
//     Npart - total number of particles to be initialized
//     r     - array of radii away from the center of the sphere (size Npart)
//     M     - total mass of the Plummer sphere
//     a     - Plummer radius (scale parameter setting the cluster core)
//     G     - gravitational constant
//     rand - RandomState generator (optional: defaults to numpy.random)

// output:
//     vel   - array of velocities of particles in Plummer model at radii r
// */
// std::vector<std::array<double, 3>> velDist_Plummer(int Npart, std::vector<std::array<double, 3>> r, float M, float a, float G, gsl_rng *rand) {
//     std::vector<std::array<double, 3>> vel(Npart);
//     gsl_vector *r_abs = gsl_vector_alloc(3);

//     for (int i = 0; i < Npart; i++) {
//         double r_abs = gsl_blas_dnrm2(&r[i]);
//         double vel_mod = rejTech_velPlummer(r_abs, M, a, G, rand);
//         std::array<double,3> rand_vec = randUnitVector(3, r);
//         for (int j = 0; j < 3; j++) {
//             vel[i][j] = rand_vec[j] * vel_mod;
//         }
//     }

//     return vel;
// }

// /*
// Uses the rejection technique to return a velocity modulus drawn at random
// from the velocity distribution of particles at radius r in Plummer model

// input:
//     r - radius away from the center of the Plummer sphere
//     M - total mass of the Plummer sphere
//     a - Plummer radius (scale parameter setting the cluster core)
//     G - gravitational constant
//     rand - RandomState generator (optional: defaults to numpy.random)

// output:
//     absoulte velocity of a particle at radius r inside a Plummer sphere
// */
// double rejTech_velPlummer(double r, double M, double a, double G, gsl_rng *rand) {
//     double x0 = 0;
//     double gmax = 0.1; // slightly bigger than g_max = g_max = g(\sqrt(2/9)) = 0.092
//     double g0 = gmax;

//     while (g0 > g_Plummer(x0)) {
//         x0 = gsl_rng_uniform(rand); // 0 <= v <= v_esc or 0 <= x <= 1 where x = v/v_esc
//         g0 = gsl_rng_uniform(rand) * gmax; // 0 <= g0 <= gmax
//     }

//     return x0 * velEscape_Plummer(r, M, a, G);
// }

// // Plummer gravitational acceleration profile
// double g_Plummer(double x) {
//     return pow(x * x * (1 - x * x), 3.5);
// }

// /*
// Returns the escape velocity of a particle in the Plummer model
// at a radius r from the center of the Plummer sphere

// input:
//     r - radius away from the center of the Plummer sphere
//     M - total mass of the Plummer sphere
//     a - Plummer radius (scale parameter setting the cluster core)
//     G - gravitational constant
// output:
//     escape velocity of a particle at radius r inside a Plummer sphere
// */
// double velEscape_Plummer(double r, double M, double a, double G) {
//     double pref = sqrt(2 * G * M/a);

//     return pref * (pow(1 + (r * r)/(a * a), -0.25));
// }