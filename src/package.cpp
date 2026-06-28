#include "package.hxx"
#include <iostream>

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package& Package::operator=(Package&& package) noexcept {
    if (this == &package)
        return *this;
    if (ID_ != 0) {
        assigned_IDs.erase(ID_);
        freed_IDs.insert(ID_);
    }
    ID_ = package.ID_;
    if (ID_ != 0) {
        assigned_IDs.insert(ID_);
    }
    package.ID_ = 0;
    return *this;
}

Package::Package() {
    if (freed_IDs.empty()) {
        if (assigned_IDs.empty()) {
            ID_ = 1;
        } else {
            ID_ = *assigned_IDs.end() + 1;
        }
    } else {
        ID_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    }
    assigned_IDs.insert(ID_);
}

Package::~Package() {
    if (ID_ == 0) return;
    freed_IDs.insert(ID_);
    assigned_IDs.erase(ID_);
}