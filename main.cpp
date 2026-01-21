#include <iostream>
#include "factory.hxx"
#include "simulation.hxx"
#include "reports.hxx"

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
