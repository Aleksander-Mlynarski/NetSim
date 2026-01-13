//
// Created by alekm on 09.01.2026.
//
#include "nodes.hxx"
#include "package.hxx"
#include "storage_types.hxx"
#include "types.hxx"
#include "config.hxx"
#include <memory>
#include <optional>
#include <map>

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_[r] = 0;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) return nullptr;
    return preferences_.begin()->first;
}

const ReceiverPreferences::preferences_t&
ReceiverPreferences::get_preferences() const {
    return preferences_;
}


void PackageSender::send_package() {

}
//const get_sending_buffer()
void Storehouse::receive_package(Package&& p) {

}

void Worker::do_work(Time t){
  }
void Worker::receive_package(Package&& p){}

ElementID Worker::get_id() const {}

void Ramp::deliver_goods(Time t){

  }




