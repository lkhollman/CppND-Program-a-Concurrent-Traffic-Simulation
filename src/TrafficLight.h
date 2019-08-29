#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

//FP.1
enum TrafficLightPhase{  
    red,
    green
};

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue
{
public:
    void send(T &&message);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
    
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

class TrafficLight : public TrafficObject
{
public:
    TrafficLight();
    void setCurrentPhase(TrafficLightPhase);
    TrafficLightPhase getCurrentPhase();  //FP.1
    void waitForGreen();  //FP.1
    void simulate();   //FP.1


    // constructor / desctructor

    // getters / setters

    // typical behaviour methods

private:
    void cycleThroughPhases();  //FP.1
    TrafficLightPhase _currentPhase;  //FP.1

//from HORKI
    std::shared_ptr<MessageQueue<TrafficLightPhase>> _messages{  //changed _mq to _messages
    new MessageQueue<TrafficLightPhase>
  };

    
    
    // typical behaviour methods

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.


    std::condition_variable _condition;  //provided
    std::mutex _mutex;   //provided
};

#endif