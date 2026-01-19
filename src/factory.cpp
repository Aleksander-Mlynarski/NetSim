//
// Created by mateuszl on 9.01.2026.
//

#include <stdexcept>
#include "factory.hxx"

void Factory::do_work(Time t){
    for (auto& worker:workers_){
        worker.do_work(t);
    }
}

void Factory::do_deliveries(Time t){
    for (auto& ramp: ramps_){
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing(){
    for (auto& ramp: ramps_){
        ramp.send_package();
    }
    for (auto& worker:workers_){
        worker.send_package();
    }
}

void Factory::remove_receiver(NodeCollection<Worker>& workers, ElementID id){
    auto it = workers.find_by_id(id);
    if (it == workers.end()) return;

    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*it));

    for (auto& ramp: ramps_){
        ramp.receiver_preferences_.remove_receiver(receiver_ptr);
    }
    for (auto& worker : workers_) {
        worker.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    workers.remove_by_id(id);
}


void Factory::remove_receiver(NodeCollection<Storehouse>& storehouses, ElementID id){
    auto it = storehouses.find_by_id(id);
    if (it == storehouses.end()) return;

    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*it));

    for (auto& ramp: ramps_){
        ramp.receiver_preferences_.remove_receiver(receiver_ptr);;
    }
    for (auto& worker : workers_) {
        worker.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    storehouses.remove_by_id(id);
}

bool Factory:: has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) return true;

    node_colors[sender] = NodeColor::VISITED;

    auto& prefs = sender->receiver_preferences_.get_preferences();
    if (prefs.empty()) {
        throw std::logic_error("Sender don't have any receiver");
    }

    bool has_path_to_storehouse = false;
    bool has_receiver_other_than_self = false;

    for (auto& [receiver, probability] : prefs) {
        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) {
            has_path_to_storehouse = true;
            has_receiver_other_than_self = true;
        }
        else if (receiver->get_receiver_type() == ReceiverType::WORKER) {
            Worker* worker_ptr = dynamic_cast<Worker*>(receiver);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) {
                continue;
            }

            has_receiver_other_than_self = true;

            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                if (has_reachable_storehouse(sendrecv_ptr, node_colors)) {
                    has_path_to_storehouse = true;
                }
            } else if (node_colors[sendrecv_ptr] == NodeColor::VERIFIED) {
                has_path_to_storehouse = true;
            }
        }
    }

    if (!has_path_to_storehouse || !has_receiver_other_than_self) {
        throw std::logic_error("There is no path from sender to storehouse");
    }

    node_colors[sender] = NodeColor::VERIFIED;
    return true;
}

bool Factory::is_consistent() const{
    std::map<const PackageSender*, NodeColor> node_colors;

    for (const auto& ramp : ramps_) {
        node_colors[&ramp] = NodeColor::UNVISITED;
    }
    for (const auto& worker : workers_) {
        node_colors[&worker] = NodeColor::UNVISITED;
    }

    try {
        for (const auto& ramp : ramps_) {
            has_reachable_storehouse(&ramp, node_colors);
        }
    } catch (const std::logic_error&) {
        return false;
    }

    return true;
}
