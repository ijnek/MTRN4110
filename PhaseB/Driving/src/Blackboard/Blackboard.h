#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../Odometry/Odometry.h"
#include "../MovementRequest/MovementRequest.h"
#include "../Vector/Vector.h"
#include "../Pose/Pose.h"

class Blackboard
{
  public:
    Blackboard(): gyroZ(0){}

    Odometry odometryDiff;  // difference in odometry since last cycle
    MovementRequest movementRequest;  // current requested velocity
    Pose worldPose;  // pose relative to where we started (world(0, 0, 0) = where we started)
    float gyroZ;  // gyro z value in rad/s
};

#endif // BLACKBOARD_H