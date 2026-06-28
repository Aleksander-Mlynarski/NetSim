#ifndef PACKAGE_HXX
#define PACKAGE_HXX

#include "types.hxx"
#include <set>

class Package {
public:
    Package();

    Package(ElementID ID) : ID_(ID) { assigned_IDs.insert(ID_); }

    Package(Package&& package) noexcept : ID_(package.ID_) { package.ID_ = 0; }

    Package& operator=(Package&& package) noexcept;

    ElementID get_id() const { return ID_; }

    ~Package();

private:
    ElementID ID_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //PACKAGE_HXX