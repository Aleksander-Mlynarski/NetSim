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

<<<<<<< Updated upstream
class Storehouse{
  public:
     ~Storehouse() = default;
     Storehouse(ElementID id,
               std::unique_ptr<IPackageStockpile> d);
=======
class ReceiverPreferences{

public:
  using preferences_t = std::map<IPackageReceiver*, double>;
  using const_iterator = preferences_t::const_iterator;
  ~ReceiverPreferences() = default;
  ReceiverPreferences(ProbabilityGenerator pg): pg_(pg) {}
  void add_receiver(IPackageReceiver* r);
  void remove_receiver(IPackageReceiver* r);
  IPackageReceiver* choose_receiver();
  const preferences_t& get_preferences() const;

private:
  preferences_t preferences_;   //modified class fields
  ProbabilityGenerator pg_;
};

class PackageSender{
public:
  ~PackageSender() = default;
  PackageSender() = default; // add default constructor
  PackageSender(PackageSender&&) = default;
  void send_package();
  const std::optional<Package>& get_sending_buffer() const; // Gives read-only access to the sending buffer.
  // No changes to the object are allowed here.
  void push_package(Package&&);
private:
  ReceiverPreferences receiver_preferences_;
  // protected: std::optional<Package> buffer_;
};

class Storehouse: public IPackageReceiver{
  public:
     ~Storehouse() = default;
     Storehouse(ElementID id,
               std::unique_ptr<IPackageStockpile> d): id_(id), d_(std::move(d)) {}
  void receive_package(Package&& p) override;
  ElementID get_id() const override;  // add declaration of abstract methods

#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
  ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;} //add implementation of abstract method
#endif

private:
  ElementID id_;
  std::unique_ptr<IPackageStockpile> d_; // add class fields
>>>>>>> Stashed changes
};
class Worker{
  public:
    ~Worker() = default;
    Worker(ElementID id,
           TimeOffset pd,
<<<<<<< Updated upstream
           std::unique_ptr<IPackageQueue> q);
     void do_work(Time t);
     TimeOffset get_processing_duration() const; //getter
     Time get_package_processing_start_time() const;//getter
};
class Ramp{
=======
           std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {}
    void do_work(Time t);
    TimeOffset get_processing_duration() const{return pd_;}
    //getter
    Time get_package_processing_start_time() const{return package_processing_start_time_;}//getter
    void receive_package(Package&& p) override;  // add declaration of abstract methods
    ElementID get_id() const override;

    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
      ReceiverType get_receiver_type() const override{return ReceiverType::WORKER;} // add implementation of abstract method
    #endif

    private:
      ElementID id_; // add fields
      TimeOffset pd_;
      std::unique_ptr<IPackageQueue> q_;
      Time package_processing_start_time_ = 0;
      std::optional<Package> processing_buffer_;
    };
class Ramp: public PackageSender{
>>>>>>> Stashed changes
  public:
    ~Ramp() = default;
    Ramp(ElementID id,
         TimeOffset di): id_(id), di_(di) {}
    void deliver_goods(Time t);
<<<<<<< Updated upstream
    TimeOffset get_delivery_interval() const;
    ElementID get_id() const;
=======
    TimeOffset get_delivery_interval() const{return  di_;} //getter
    ElementID get_id() const; // getter
private:
  ElementID id_;  // add fields
  TimeOffset di_;
>>>>>>> Stashed changes
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
