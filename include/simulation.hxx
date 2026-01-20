//
// Created by Milosz on 20.01.2026.
//

#ifndef SIMULATION_HXX
#define SIMULATION_HXX

#include <functional>
#include <stdexcept>
#include "types.hxx"

class Factory;

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf);

#endif // SIMULATION_HXX
