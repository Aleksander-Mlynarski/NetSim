//
// Created by alekm on 09.01.2026.
//

#ifndef HELPERS_HXX
#define HELPERS_HXX
#include <functional>
#include <random>

#include "types.hxx"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //HELPERS_HXX
