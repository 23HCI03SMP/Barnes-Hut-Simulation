#include <array>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <list>
#include <vector>
#include <iostream>

// Pi constant
constexpr double pi() { return std::acos(-1); }

std::vector<double> randUnitVector(int d, gsl_rng *r);
std::vector<std::array<int, 3>> plummer(int Npart, double a, double m, double G, int seed);
std::vector<int> plummerDist_3d_xyz(int Npart, double a, int seed);