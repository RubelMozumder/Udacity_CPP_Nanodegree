#include "TrafficLight.h"

#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

template <class T>
T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> MQloc(_MQmutx);
  _MQcon_var.wait(MQloc, [this] { return !_mq.empty(); });
  T msg = std::move(_mq.back());
  _mq.pop_back();
  return msg;
};

template <class T>
void MessageQueue<T>::send(T &&msg) {
  std::lock_guard<std::mutex> MQloc(_MQmutx);
  _mq.push_back(std::move(msg));
  _MQcon_var.notify_one();
};

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() {
  _currentPhase = TrafficLightPhase::red;
  _type = trafficLight;
}

void TrafficLight::waitForGreen() {
  while (true) {
    TrafficLightPhase phase = MQ.receive();
    if (phase == TrafficLightPhase::green) break;
  };
};

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() { cycleThroughPhases(); };

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  double update = 0;
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<> distr(4000.0, 7000.0);
  double green_t = distr(eng);
  while (true) {
    start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(10));

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_elapsed = end - start;
    update += time_elapsed.count();

    if (getCurrentPhase() == TrafficLightPhase::red && update < green_t) {
      _currentPhase = TrafficLightPhase::green;
      MQ.send(std::move(TrafficLightPhase::green));

    } else if (getCurrentPhase() == TrafficLightPhase::green &&
               update >= green_t) {
      _currentPhase = TrafficLightPhase::red;
      MQ.send(std::move(TrafficLightPhase::red));

    } else if (update >= 2 * green_t) {
      double green_t = distr(eng);
      update = 0.0;
    };
  }
};
