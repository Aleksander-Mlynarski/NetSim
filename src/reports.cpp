//
// Created by Lenovo on 19.01.2026.
//

#include "reports.hxx"
#include <vector>
#include <stdexcept>

ParsedLineData parse_line(std::string line){
  ParsedLineData Data;
  std:: vector<std::string> tokens;
  std:: string wyraz = "";
  for (auto it = line.begin(); it != line.end(); it++) {
    if (*it == ' '){
      if(wyraz != "") tokens.push_back(wyraz);
      wyraz = "";
    }
    else{
      wyraz += *it;
    }
  }
  if (map.count(tokens[0])){
    Data.type = map[tokens[0]];
  }
  else{
    throw std::runtime_error("Invalid element type");
  }
  for (auto& token : tokens){
    if (token == tokens[0]) continue;
    std::string klucz = "";
    std::string wartosc = "";
    bool z = false;
    for(auto it = token.begin(); it != token.end(); it++){
      if (*it == '='){
        z = true;
      }
      if(z) klucz += *it;
      else wartosc += *it;
    }
    Data.data.insert({klucz, wartosc});
  }
  return Data;
}

Factory load_factory_structure(std::istream& is){
  Factory factory;
  std::string line;
  while (std::getline(is, line)){
    auto first_char_it = std::find_if_not(line.begin(), line.end(),
                                          [](unsigned char c) {return std::isspace(c);});
    if(first_char_it == line.end() || *first_char_it == ';') continue;
    ParsedLineData parseLine = parse_line(line);
    switch (parseLine.type) {
    case ElementType::RAMP: {
      ElementID idr = std::stoi(parseLine.data.at("id"));
      TimeOffset delivery_time = std::stoi(parseLine.data.at("delivery-interval"));
      Ramp r(idr, delivery_time);
      factory.add_ramp(std::move(r));
      break;}
    case ElementType::WORKER:{
      ElementID idw = std::stoi(parseLine.data.at("id"));
      Time pt = std::stoi(parseLine.data.at("processing-time"));

      std::string q_type_str = parseLine.data.at("queue-type");
      PackageQueueType q_type;
      if (q_type_str == "FIFO") {
        q_type = PackageQueueType::FIFO;
      } else if (q_type_str == "LIFO") {
        q_type = PackageQueueType::LIFO;
      } else {
        throw std::runtime_error("Unknown queue type: " + q_type_str);
      }
      factory.add_worker(Worker(idw, pt, std::make_unique<PackageQueue>(q_type)));
      break;}
    case ElementType::STOREHOUSE:{
      ElementID ids = std::stoi(parseLine.data.at("id"));
      //factory.add_storehouse(std::move(Storehouse(ids, std::make_unique<IPackageStockpile>()))));
      break;}
    case ElementType::LINK:{
      break;}
    }
  }
  return factory;
}