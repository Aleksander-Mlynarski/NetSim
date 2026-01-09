//
// Created by mateuszl on 31.12.2025.
//

#include "nodes.hxx"
#include "types.hxx"
#include "node_collection.hxx"

#ifndef FACTORY_HXX
#define FACTORY_HXX

class Factory{
  NodeCollection<Ramp> ramps_;
  NodeCollection<Worker> workers_;
  NodeCollection<Storehouse> storehouses_;
public:
  Factory() = default;
  ~Factory() = default;
  void add_ramp(Ramp&& ramp);
  void add_worker(Worker&& worker);
  void add_storehouse(Storehouse&& storehouse);
  void remove_ramp(ElementID id);
  void remove_worker(ElementID id);
  void remove_storehouse(ElementID id);
  NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
  NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
  NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
  NodeCollection<Ramp>::const_iterator find_ramp_by_id (ElementID id) const;
  NodeCollection<Worker>::const_iterator find_worker_by_id (ElementID id) const;
  NodeCollection<Storehouse>::const_iterator find_storehouse_by_id (ElementID id) const;
  NodeCollection<Ramp>::const_iterator ramp_cbegin() const;
  NodeCollection<Worker>::const_iterator worker_cbegin() const;
  NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const;
  NodeCollection<Ramp>::const_iterator ramp_cend() const;
  NodeCollection<Worker>::const_iterator worker_cend() const;
  NodeCollection<Storehouse>::const_iterator storehouse_cend() const;

  void remove_receiver(NodeCollection<Worker>&, ElementID id);
  void remove_receiver(NodeCollection<Storehouse>&, ElementID id);

  bool is_consistent() const;
  void do_deliveries(Time t);
  void do_package_passing();
  void do_work(Time t);
};

#endif //FACTORY_HXX
