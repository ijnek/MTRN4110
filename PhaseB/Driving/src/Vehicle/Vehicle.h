#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "../MovementRequest/MovementRequest.h"
#include "../Blackboard/Blackboard.h"

class Vehicle
{
  public:
    Vehicle(Blackboard &blackboard)
      : blackboard(blackboard) {}
      
    void tick();

  private:
    Blackboard &blackboard;
    MovementRequest currentCommand;
    
    void calculateWheelAngularVelocities(MovementRequest &movementRequest, float &leftWheelAngularVelocity, float &rightWheelAngularVelocity);
    Odometry calculateOdometryDiff(int leftCounter, int rightCounter);  // calculate odometry difference since last tick
};

static void leftEncoderInterrupt();
static void rightEncoderInterrupt();

#endif // VEHICLE_HPP