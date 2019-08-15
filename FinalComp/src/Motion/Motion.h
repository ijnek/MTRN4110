#ifndef MOTION_H
#define MOTION_H

#include "../Types/MovementRequest.h"
#include "../Types/Blackboard.h"

class Motion
{
  public:
    Motion(Blackboard &blackboard)
      : blackboard(blackboard) {}
      
    void tick();
    void reset();

  private:
    Blackboard &blackboard;
    
    void calculateWheelAngularPositions(MovementRequest &movementRequest, float &leftWheelAngularVelocity, float &rightWheelAngularVelocity);
    Odometry calculateOdometryDiff(int leftCounter, int rightCounter);  // calculate odometry difference since last tick
};

static void leftEncoderInterrupt();
static void rightEncoderInterrupt();

#endif // MOTION_H