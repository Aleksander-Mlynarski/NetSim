#include <fstream>
#include <iostream>
#include "factory.hxx"
#include "simulation.hxx"
#include "reports.hxx"

int main() {
    // PARAMETRY SYMULACJI
    const char* factory_file = "examples/factory.txt";  // sciezka do pliku z siecia
    const TimeOffset simulation_turns = 10;               // ile tur symulowac
    const TimeOffset report_every_n_turns = 2;            // raport co N tur (0 = wylaczone)

    // Wczytanie fabryki
    Factory f;
    std::ifstream in(factory_file);
    if (in) {
        f = load_factory_structure(in);
    } else {
        std::cerr << "Nie znaleziono pliku: " << factory_file
                  << " — uruchamiam pusta fabryke.\n";
    }

    IntervalReportNotifier notifier(report_every_n_turns);

    simulate(f, simulation_turns, [&](Factory& fac, Time t) {
        if (report_every_n_turns > 0 && notifier.should_generate_report(t)) {
            generate_simulation_turn_report(fac, std::cout, t);
        }
    });

    return 0;
}
