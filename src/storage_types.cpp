#include <iostream>
#include "storage_types.hxx"

Package PackageQueue::pop() {
    Package package;

    if (this->package_queue_type_ == PackageQueueType::LIFO) {
        package = std::move(this->package_list_.back());
        this->package_list_.pop_back();
    } else {
        package = std::move(this->package_list_.front());
        this->package_list_.pop_front();
    }

    return package;
}
std::list<Package> package_list_;
