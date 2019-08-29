#include <iostream>
#include <random>
#include <future>  //I added
#include <chrono> // I added
#include "TrafficLight.h"



/* Implementation of class "MessageQueue" */

//Entire file was commented out.  I uncommented entire file.


template <typename T>
T MessageQueue<T>::receive() {
  // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
  // to wait for and receive new messages and pull them from the queue using move semantics.
  // The received object should then be returned by the receive function.

  
  std::unique_lock<std::mutex> ULock(_mutex);  // modify queue while locked
  _condition.wait(ULock, [this] { return !_queue.empty(); });  // pass unique lock to condition variable

  
  T message = std::move(_queue.back());
  _queue.pop_back();  // remove last element from queue

  return message;
}



template <typename T>
void MessageQueue<T>::send(T && message) {
  // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
  // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

  // perform queue modification under the lock
  std::lock_guard<std::mutex> ULock(_mutex);  //changed uLock to ULock

  // add vector to queue
  std::cout << "   Message " << message << " has been sent to the queue\n";
  _queue.push_back(std::move(message));
  // notify client after pushing new Vehicle into vector
  _condition.notify_one();
}

/* Implementation of class "TrafficLight" */
TrafficLight::TrafficLight(){
    _currentPhase = TrafficLightPhase::red;
}



void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

   
    while (1) {
    TrafficLightPhase message = _messages->receive();  
    if (message == TrafficLightPhase::green){  //wait for light to turn green
        return;    //before returning
    }   
  }                                              
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate() {
  // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


//HORKI
void TrafficLight::cycleThroughPhases() {
  // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
  // and toggles the current phase of the traffic light between red and green and sends an update method
  // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
  // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
  std::chrono::time_point<std::chrono::system_clock> lastUpdate =
    std::chrono::system_clock::now();  //get current time and store in lastUpdate
  std::random_device randomDevice;
  std::mt19937 rand(randomDevice());  
  std::uniform_int_distribution<int> distr(4000, 6000);  //get random time between 4 & 6 seconds
  int cycleDuration = distr(rand);   //store random 4-6 second time in cycleDuration

  //infinite loop
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  //wait for 1ms between 2 cycles

    int lastUpdatedTime =
      std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - lastUpdate).count();   //length of time since last iteration
    
    if (lastUpdatedTime >= cycleDuration) {  
      cycleDuration = distr(rand);  //get a new 4-6 second random time
      lastUpdate = std::chrono::system_clock::now(); //get current time

      _currentPhase = (_currentPhase == TrafficLightPhase::green) ? TrafficLightPhase::red : 
        TrafficLightPhase::green;
      
      TrafficLightPhase message = _currentPhase;  
      
      std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send, _messages, 
        std::move(message));  
    }
  }
}
