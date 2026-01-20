//
// Created by Lenovo on 19.01.2026.
//
#include <iostream>
#include "reports.hxx"
#include <vector>
#include <stdexcept>

ParsedLineData parse_line(std::string line){
  ParsedLineData Data;
  std:: vector<std::string> tokens;
  std:: string wyraz = "";
  for (auto it = line.begin(); it != line.end(); it++) {
    if (*it == ' '|| *it == '\t'){
      if(wyraz != "") tokens.push_back(wyraz);
      wyraz = "";
    }
    else{
      wyraz += *it;
    }
  }
  if(wyraz != "") tokens.push_back(wyraz);

  if (tokens.empty()) throw std::runtime_error("Empty line");

  if (mapa.count(tokens[0])){
    Data.type = mapa[tokens[0]];
  }
  else{
    throw std::runtime_error("Invalid element type"+ tokens[0]);
  }
  for (auto& token : tokens){
    if (token == tokens[0]) continue;
    std::string klucz = "";
    std::string wartosc = "";
    bool z = false;
    for(auto it = token.begin(); it != token.end(); it++){
      if (*it == '='){
        z = true;
        continue;
      }
      if(!z) klucz += *it;
      else wartosc += *it;
    }
    Data.data.insert({klucz, wartosc});
  }
  return Data;
}

void handle_link(const ParsedLineData& pld, Factory& factory) {
    std::string src = pld.data.at("src");
    std::string dest = pld.data.at("dest");

    auto split_id = [](const std::string& s) {
        size_t pos = s.find('-');
        return std::make_pair(s.substr(0, pos), std::stoi(s.substr(pos + 1)));
    };

    auto [src_type, src_id] = split_id(src);
    auto [dest_type, dest_id] = split_id(dest);

    IPackageReceiver* receiver;
    if (dest_type == "worker") {
        receiver = &(*factory.find_worker_by_id(dest_id));
    } else {
        receiver = &(*factory.find_storehouse_by_id(dest_id));
    }

    if (src_type == "ramp") {
        factory.find_ramp_by_id(src_id)->receiver_ptr.add_receiver(receiver);
    } else if (src_type == "worker") {
        factory.find_worker_by_id(src_id)->receiver_ptr.add_receiver(receiver);
    }
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
        factory.add_storehouse(Storehouse(ids));
        break;}
    case ElementType::LINK:{
      handle_link(parseLine, factory);
      break;}
    }
  }
  return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    os << "; == LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }

    os << "\n; == WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_time()
           << " queue-type=" << (it->get_queue()->get_queue_type() == PackageQueueType::LIFO ? "LIFO" : "FIFO") << "\n";
    }

    os << "\n; == STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    os << "\n; == LINKS ==\n\n";
    auto print_links = [&](auto& node, std::string type_prefix) {
        auto receivers = node.receiver_ptr.get_all_receivers();
        for (auto& rec : receivers) {
            std::string dest_type = (rec->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "store";
            os << "LINK src=" << type_prefix << "-" << node.get_id()
               << " dest=" << dest_type << "-" << rec->get_id() << "\n";
        }
    };

    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) print_links(*it, "ramp");
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) print_links(*it, "worker");

    os.flush();
}

void generate_structure_report(Factory& factory, std::ostream& os) {
    os << "== LOADING RAMPS ==\n\n";

    os.flush();
}
static std::string receiver_label(const IPackageReceiver& r) {
    if (r.get_receiver_type() == ReceiverType::WORKER) {
        return "worker #" + std::to_string(r.get_id());
    }
    return "storehouse #" + std::to_string(r.get_id());
}

static int receiver_rank(const IPackageReceiver& r) {
    if (r.get_receiver_type() == ReceiverType::STOREHOUSE) return 1;
    return 2;
}

static std::vector<const IPackageReceiver*> sorted_receivers(const ReceiverPreferences& prefs) {
    std::vector<const IPackageReceiver*> out;
    for (const auto& [r, _] : prefs.get_preferences()) {
        out.push_back(r);
    }
    std::sort(out.begin(), out.end(), [](const IPackageReceiver* a, const IPackageReceiver* b) {
        int ra = receiver_rank(*a);
        int rb = receiver_rank(*b);
        if (ra != rb) return ra < rb;
        return a->get_id() < b->get_id();
    });
    return out;
}

static void print_receivers(std::ostream& os, const ReceiverPreferences& prefs) {
    os << "  Receivers:\n";
    for (const auto* r : sorted_receivers(prefs)) {
        os << "    " << receiver_label(*r) << "\n";
    }
}

static void print_package_list(std::ostream& os,
                               std::list<Package>::const_iterator b,
                               std::list<Package>::const_iterator e) {
    if (b == e) {
        os << "(empty)";
        return;
    }
    bool first = true;
    for (auto it = b; it != e; ++it) {
        if (!first) os << ", ";
        first = false;
        os << "#" << it->get_id();
    }
}

void generate_structure_report(const Factory& f, std::ostream& os) {
    os << "== LOADING RAMPS ==\n\n";
    for (const auto& r : f.get_loading_ramps()) {
        os << "LOADING RAMP #" << r.get_id() << "\n";
        os << "  Delivery interval: " << r.get_delivery_interval() << "\n";
        print_receivers(os, r.receiver_preferences());
        os << "\n";
    }

    os << "== WORKERS ==\n\n";
    for (const auto& w : f.get_workers()) {
        os << "WORKER #" << w.get_id() << "\n";
        os << "  Processing time: " << w.get_processing_duration() << "\n";
        os << "  Queue type: "
           << (w.get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO")
           << "\n";
        print_receivers(os, w.receiver_preferences());
        os << "\n";
    }

    os << "== STOREHOUSES ==\n\n";
    for (const auto& s : f.get_storehouses()) {
        os << "STOREHOUSE #" << s.get_id() << "\n\n";
    }
}

static void print_worker_state(std::ostream& os, const Worker& w) {
    os << "WORKER #" << w.get_id() << "\n";

    os << "  PBuffer: ";
    const auto& pb = w.get_processing_buffer();
    if (!pb.has_value()) {
        os << "(empty)\n";
    } else {
        os << "#" << pb->get_id() << " (pt = " << w.get_processing_time() << ")\n";
    }

    os << "  Queue: ";
    const auto* q = w.get_queue();
    print_package_list(os, q->cbegin(), q->cend());
    os << "\n";

    os << "  SBuffer: ";
    const auto& sb = w.get_sending_buffer();
    if (!sb.has_value()) {
        os << "(empty)\n";
    } else {
        os << "#" << sb->get_id() << "\n";
    }

    os << "\n";
}

static void print_storehouse_state(std::ostream& os, const Storehouse& s) {
    os << "STOREHOUSE #" << s.get_id() << "\n";
    os << "  Stock: ";
    const auto& stock = s.get_stockpile();
    auto b = stock.cbegin();
    auto e = stock.cend();
    if (b == e) {
        os << "(empty)\n\n";
        return;
    }
    bool first = true;
    for (auto it = b; it != e; ++it) {
        if (!first) os << ", ";
        first = false;
        os << "#" << it->get_id();
    }
    os << "\n\n";
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {
    os << "=== [ Turn: " << t << " ] ===\n\n";

    os << "== WORKERS ==\n\n";
    for (const auto& w : f.get_workers()) {
        print_worker_state(os, w);
    }

    os << "== STOREHOUSES ==\n\n";
    for (const auto& s : f.get_storehouses()) {
        print_storehouse_state(os, s);
    }
}