//
// Created by mateuszl on 9.01.2026.
//

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