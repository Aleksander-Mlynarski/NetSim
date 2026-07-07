//
// Created by Lenovo on 19.01.2026.
//

#ifndef REPORTS_HXX
#define REPORTS_HXX
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <ostream>
#include <istream>
#include "factory.hxx"

enum class ElementType {RAMP, WORKER, STOREHOUSE, LINK};

inline std::map<std::string, ElementType> mapa = {{"LOADING_RAMP",ElementType::RAMP}, {"WORKER", ElementType::WORKER},
                                          {"STOREHOUSE", ElementType::STOREHOUSE}, {"LINK", ElementType::LINK}};

struct ParsedLineData{
  ElementType type;
  std::map<std::string, std::string> data;
};

ParsedLineData parse_line(std::string line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

class Factory;

class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns)
        : turns_(std::move(turns)) {}

    bool should_generate_report(Time t) const {
        return turns_.find(t) != turns_.end();
    }

private:
    std::set<Time> turns_;
};

class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier(TimeOffset interval)
        : interval_(interval) {}

    bool should_generate_report(Time t) const {
        if (interval_ <= 0) return false;
        return ((t - 1) % interval_) == 0;
    }

private:
    TimeOffset interval_;
};

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);


#endif //REPORTS_HXX
