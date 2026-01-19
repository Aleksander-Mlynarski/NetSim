//
// Created by Lenovo on 19.01.2026.
//

#ifndef REPORTS_HXX
#define REPORTS_HXX
#include <map>
#include <string>
#include <algorithm>
#include <ostream>
#include <istream>
#include "factory.hxx"

enum class ElementType {RAMP, WORKER, STOREHOUSE, LINK};

std:: map<std::string, ElementType> map = {{"LOADING_RAMP",ElementType::RAMP}, {"WORKER", ElementType::WORKER},
                                          {"STOREHOUSE", ElementType::STOREHOUSE}, {"LINK", ElementType::LINK}};

struct ParsedLineData{
  ElementType type;
  std::map<std::string, std::string> data;
};

ParsedLineData parse_line(std::string line);

Factory load_factory_structure(std::istream& is);

#endif //REPORTS_HXX
