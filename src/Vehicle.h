#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Street;
class Intersection;

class Vehicle : public TrafficObject, public std::enable_shared_from_this<Vehicle>
{                                     //in place of using "this", because they are
public:                               //shared pointers
    // constructor / desctructor
    Vehicle();

    // getters / setters
    void setCurrentStreet(std::shared_ptr<Street> street) { _currStreet = street; };
    void setCurrentDestination(std::shared_ptr<Intersection> destination);

    // typical behaviour methods
    void simulate();

    // miscellaneous
    std::shared_ptr<Vehicle> get_shared_this() { return shared_from_this(); }  //keyword "this"
           //cannot be used with shared pointers, so this concept is used
private:
    // typical behaviour methods
    void drive();

    std::shared_ptr<Street> _currStreet;            // street on which the vehicle is currently on
    std::shared_ptr<Intersection> _currDestination; // destination to which the vehicle is currently driving
    double _posStreet;                              // position on current street
    double _speed;                                  // ego speed in m/s
};

#endif