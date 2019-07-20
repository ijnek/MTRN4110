#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../Odometry/Odometry.h"
#include "../MovementRequest/MovementRequest.h"
#include "../Vector/Vector.h"
#include "../Pose/Pose.h"

class Blackboard
{
  public:
    Blackboard(){}

    Odometry odometryDiff;  // difference in odometry since last cycle
    MovementRequest movementRequest;  // current requested velocity
    Pose worldPose;  // pose relative to where we started (world(0, 0, 0) = where we started)
};

#endif // BLACKBOARD_H