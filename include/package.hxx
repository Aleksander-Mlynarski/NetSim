#include "types.hxx"

class Package {
  ElementID id_;
  public:
    Package() = default;
    Package(ElementID id) : id_(id) {}
    Package(const Package &) = default;
    Package(Package &&) = default;
    ElementID get_id() const {return id_;}
    ~Package() = default;
};
