#ifndef EVENT_PRODUCER_HH
#define EVENT_PRODUCER_HH

// C++ standard library
#include <chrono>
#include <iostream>
#include <vector>

#include <boost/format.hpp>

// enum for selecting which events to include
enum EventType { Gluon, Quark, All };

struct Particle {
  Particle(double pt_, double y_, double phi_) : pt(pt_), y(y_), phi(phi_) {}
  double pt, y, phi;
};

// base class for producing events
class EventProducer {
protected:

  long num_events_;
  unsigned tot_num_events_, print_every_, iEvent_, iAccept_;
  EventType event_type_;
  std::chrono::steady_clock::time_point start_;

  std::vector<Particle> particles_;
  double weight_;

public:

  EventProducer(long num_events, unsigned print_every, EventType event_type = All) :
    num_events_(num_events),
    print_every_(print_every),
    iEvent_(0), 
    iAccept_(0),
    event_type_(event_type),
    start_(std::chrono::steady_clock::now()),
    weight_(1)
  {}

  void print_loaded() {
    std::cout << boost::format("Loaded dataset in %4.2fs\n") % duration() << std::flush;
    start_ = std::chrono::steady_clock::now();
  }

  void print_progress(bool exact = false) const {
    if (exact)
      std::cout << boost::format("%i / %i / %i - %4.2fs\n") 
                                 % iAccept_ % iEvent_ % tot_num_events_
                                 % duration();
    else
      std::cout << boost::format("%ik / %ik / %ik - %4.2fs\n") 
                                 % (iAccept_/1000) % (iEvent_/1000) % (tot_num_events_/1000)
                                 % duration();
    std::cout << std::flush;
  }

  // advances the internals to the next event
  virtual bool next() = 0;
  void reset() { iEvent_ = 0; }
  
  // accessor functions
  const std::vector<Particle> & particles() const { return particles_; }
  double weight() const { return weight_; }

private:

  double duration() const {
    auto diff(std::chrono::steady_clock::now() - start_);
    return std::chrono::duration_cast<std::chrono::duration<double>>(diff).count();
  }

}; // EventProducer

#endif // EVENT_PRODUCER_HH