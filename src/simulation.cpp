//
// Created by Milosz on 20.01.2026.
//

#include <iostream>
#include "simulation.hxx"
#include "factory.hxx"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    if (!f.is_consistent()) {
        throw std::logic_error("simulate(): factory network is not consistent");
    }

    for (Time t = 1; t <= d; ++t) {
        // 1) Dostawa
        f.do_deliveries(t);

        // 2) Przekazanie
        f.do_package_passing();

        // 3) Przetworzenie
        f.do_work(t);

        // 4) Raportowanie
        if (rf) {
            rf(f, t);
        }
    }
}