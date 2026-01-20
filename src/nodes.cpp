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
    preferences_[r] = 1.0;
    double p = 1.0 / preferences_.size();
    for (auto& [_, prob] : preferences_) {
        prob = p;
        }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
    if (preferences_.empty()) return;

    double p = 1.0 / preferences_.size();
    for (auto& [_, prob] : preferences_) {
        prob = p;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) return nullptr;

    double p = pg_();   // ⬅️ kluczowe
    double acc = 0.0;

    for (const auto& [receiver, prob] : preferences_) {
        acc += prob;
        if (p <= acc) {
            return receiver;
        }
    }
    return preferences_.begin()->first;
}

const ReceiverPreferences::preferences_t& ReceiverPreferences::get_preferences() const {
    return preferences_;
}


void PackageSender::send_package() {
    if (!buffer_) return;

    IPackageReceiver* r = receiver_preferences_.choose_receiver();
    if (!r) return;

    r->receive_package(std::move(*buffer_));
    buffer_.reset();
}
void Storehouse::receive_package(Package&& p) {
    d_->push(std::move(p));
}
const std::optional<Package>& PackageSender::get_sending_buffer() const {
    return buffer_;
}
void Worker::do_work(Time t){
    if (!processing_buffer_) {
        if (!q_->empty()) {
            processing_buffer_.emplace(q_->pop());
            package_processing_start_time_ = t;
        }
    }

    if (processing_buffer_) {
        if (t - package_processing_start_time_ + 1 >= pd_) {
            push_package(std::move(*processing_buffer_));
            processing_buffer_.reset();
        }
    }
  }
void Worker::receive_package(Package&& p){
    q_->push(std::move(p));
}

ElementID Worker::get_id() const {
  return id_;
}

void Ramp::deliver_goods(Time t){
    if ((t - 1) % di_ == 0) {
        push_package(Package());
    }
 }
ElementID Ramp::get_id() const {
    return id_;
}
