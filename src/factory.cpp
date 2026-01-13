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


