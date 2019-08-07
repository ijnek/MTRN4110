#ifndef MOTION_HPP
#define MOTION_HPP

#include "../MovementRequest/MovementRequest.h"
#include "../Blackboard/Blackboard.h"

class Motion
{
  public:
    Motion(Blackboard &blackboard)
      : blackboard(blackboard) {}
      
    void tick();

  private:
    Blackboard &blackboard;
    MovementRequest currentCommand;
    
    void calculateWheelAngularPositions(MovementRequest &movementRequest, float &leftWheelAngularVelocity, float &rightWheelAngularVelocity);
    Odometry calculateOdometryDiff(int leftCounter, int rightCounter);  // calculate odometry difference since last tick
};

static void leftEncoderInterrupt();
static void rightEncoderInterrupt();

#endif // MOTION_HPP