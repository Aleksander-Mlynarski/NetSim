#include <iostream>
#include "factory.hpp"
#include "simulation.hpp"
#include "reports.hpp"

int main() {
    Factory f;

    IntervalReportNotifier notifier(2);

    simulate(f, 10, [&](Factory& fac, Time t) {
        if (notifier.should_generate_report(t)) {
            generate_simulation_turn_report(fac, std::cout, t);
        }
    });

    return 0;
}
