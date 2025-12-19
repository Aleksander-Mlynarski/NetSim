    //
// Created by alekm on 16.12.2025.
//
#include "package.hxx"
#include "storage_types.hxx"
#include "types.hxx"
#include "config.hxx"
#include <memory>
#include <optional>
#include <map>
#ifndef NODES_HXX
#define NODES_HXX

class IPackageReceiver {
  public:
    virtual ~IPackageReceiver() = default;
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
 ... get_receiver_type() ... // deklaracja metody
 #endif
};

class Storehouse{
  public:
     ~Storehouse() = default;
     Storehouse(ElementID id,
               std::unique_ptr<IPackageStockpile> d);
};
class Worker{
  public:
    ~Worker() = default;
     Worker(ElementID id,
           TimeOffset pd,
           std::unique_ptr<IPackageQueue> q);
     void do_work(Time t);
     TimeOffset get_processing_duration() const; //getter
     Time get_package_processing_start_time() const;//getter
};
class Ramp{
  public:
    ~Ramp() = default;
    Ramp(ElementID id,
         TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const;
    ElementID get_id() const;
};
class PackageSender{
  public:
    ~PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const; // Gives read-only access to the sending buffer.
                                                              // No changes to the object are allowed here.
	void push_package(Package&&);
  private:
    ReceiverPreferences receiver_preferences_;
    };
class ReceiverPreferences{

  public:
    using preferences_t = std::map<IPackageReceiver*, double>;
	using const_iterator = preferences_t::const_iterator;
    ~ReceiverPreferences() = default;
	ReceiverPreferences(ProbabilityGenerator pg);
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
	IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const;

  private:
    preferences_t ;
    };


#endif //NODES_HXX
