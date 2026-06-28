    //
// Created by alekm on 16.12.2025.
// Modified by mateuszl on 30.12.2025.
//
#include "package.hxx"
#include "storage_types.hxx"
#include "config.hxx"
#include "helpers.hxx"
#include <memory>
#include <optional>
#include <map>
#ifndef NODES_HXX
#define NODES_HXX

enum class ReceiverType{
      WORKER, STOREHOUSE   // add new enum class
};

class IPackageReceiver {
  public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual ~IPackageReceiver() = default;
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    virtual ReceiverType get_receiver_type() const = 0;  // add new method to the interface
    #endif
};

class ReceiverPreferences{
  public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    const_iterator begin() const { return preferences_.begin(); }
    const_iterator end() const { return preferences_.end(); }
    ReceiverPreferences() : pg_(probability_generator) {}
    ReceiverPreferences(ProbabilityGenerator pg) : pg_(pg) {}
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
    virtual ~PackageSender() = default;
    PackageSender() = default; // add default constructor
    PackageSender(PackageSender&&) = default;
    PackageSender& operator=(PackageSender&&) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const; // Gives read-only access to the sending buffer.
    // No changes to the object are allowed here.
    void push_package(Package&&);
  //private:
    ReceiverPreferences receiver_preferences_;
  protected: std::optional<Package> buffer_;
  };

  class Storehouse: public IPackageReceiver{
    public:
      ~Storehouse() = default;
      Storehouse(ElementID id);
      Storehouse(ElementID id,
                std::unique_ptr<IPackageStockpile> d): id_(id), d_(std::move(d)) {}
      Storehouse(Storehouse&&) = default;
      Storehouse& operator=(Storehouse&&) = default;
      Storehouse(const Storehouse&) = delete;
      Storehouse& operator=(const Storehouse&) = delete;
      void receive_package(Package&& p) override;
      ElementID get_id() const override;  // add declaration of abstract methods
      const_iterator begin() const override { return d_->begin(); }
      const_iterator cbegin() const override { return d_->cbegin(); }
      const_iterator end() const override { return d_->end(); }
      const_iterator cend() const override { return d_->cend(); }
#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;} //add implementation of abstract method
#endif
      const IPackageStockpile& get_stockpile() const { return *d_; }

  private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_; // add class fields
  };

  class Worker: public IPackageReceiver, public PackageSender{
  public:
    ~Worker() = default;
    Worker(ElementID id,
           TimeOffset pd,
           std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {}
    Worker(Worker&&) = default;
    Worker& operator=(Worker&&) = default;
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    void do_work(Time t);
    TimeOffset get_processing_duration() const{return pd_;}
    Time get_package_processing_start_time() const{return package_processing_start_time_;}
    const std::optional<Package>& get_processing_buffer() const { return processing_buffer_; }
    Time get_processing_time(Time t) const {
        if (!processing_buffer_) return 0;
        return t - package_processing_start_time_ + 1;
    }
    const IPackageQueue* get_queue() const { return q_.get(); }
    void receive_package(Package&& p) override;  // add declaration of abstract methods
    ElementID get_id() const override;
    const_iterator begin() const override { return q_->begin(); }
    const_iterator cbegin() const override { return q_->cbegin(); }
    const_iterator end() const override { return q_->end(); }
    const_iterator cend() const override { return q_->cend(); }

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
  public:
    ~Ramp() = default;
    Ramp(ElementID id,
         TimeOffset di): id_(id), di_(di) {}
    Ramp(Ramp&&) = default;
    Ramp& operator=(Ramp&&) = default;
    Ramp(const Ramp&) = delete;
    Ramp& operator=(const Ramp&) = delete;
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const{return  di_;} //getter
    ElementID get_id() const; // getter
  private:
    ElementID id_;  // add fields
    TimeOffset di_;
  };

#endif //NODES_HXX
