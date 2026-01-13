//
// Created by mateuszl on 31.12.2025.
//

#include <list> // std::list was chosen because it does not invalidate pointers
#include <algorithm>
#include "nodes.hxx"
#include "types.hxx"


#ifndef FACTORY_HXX
#define FACTORY_HXX

template <typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }

    const_iterator begin() const { return collection_.begin(); }
    const_iterator end() const { return collection_.end(); }

    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }

    void add(Node&& node) {
        collection_.push_back(std::move(node));
    }

    iterator find_by_id(ElementID id) {
        return std::find_if(
            collection_.begin(),
            collection_.end(),
            [id](const Node& node) {
                return node.get_id() == id;
            }
        );
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(
            collection_.cbegin(),
            collection_.cend(),
            [id](const Node& node) {
                return node.get_id() == id;
            }
        );
    }

    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != collection_.end()) {
            collection_.erase(it);
        }
    }

private:
    container_t collection_;
};


class Factory{
  NodeCollection<Ramp> ramps_;
  NodeCollection<Worker> workers_;
  NodeCollection<Storehouse> storehouses_;
public:
  Factory() = default;
  ~Factory() = default;
  void add_ramp(Ramp&& ramp) {ramps_.add(std::move(ramp));}
  void add_worker(Worker&& worker) {workers_.add(std::move(worker));}
  void add_storehouse(Storehouse&& storehouse) {storehouses_.add(std::move(storehouse));}
  void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
  void remove_worker(ElementID id);
  void remove_storehouse(ElementID id);
  NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
  NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
  NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
  NodeCollection<Ramp>::const_iterator find_ramp_by_id (ElementID id) const {return ramps_.find_by_id(id);}
  NodeCollection<Worker>::const_iterator find_worker_by_id (ElementID id) const {return workers_.find_by_id(id);}
  NodeCollection<Storehouse>::const_iterator find_storehouse_by_id (ElementID id) const {return storehouses_.find_by_id(id);}
  NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin();}
  NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin();}
  NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.cbegin();}
  NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();}
  NodeCollection<Worker>::const_iterator worker_cend() const {return workers_.cend();}
  NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses_.cend();}

  void remove_receiver(NodeCollection<Worker>&, ElementID id);
  void remove_receiver(NodeCollection<Storehouse>&, ElementID id);

  bool is_consistent() const;
  void do_deliveries(Time t);
  void do_package_passing();
  void do_work(Time t);
};

#endif //FACTORY_HXX
