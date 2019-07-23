#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../Odometry/Odometry.h"
#include "../MovementRequest/MovementRequest.h"
#include "../Vector/Vector.h"
#include "../Pose/Pose.h"
#include "../Constants/Constants.h"

class Blackboard
{
  public:
    Blackboard(): gyroZ(0), worldPose(WHEELS_OFFSET_X_MM, 0, 0), plan(4){}

    Odometry odometryDiff;  // difference in odometry since last cycle
    MovementRequest movementRequest;  // current requested velocity
    Pose worldPose;  // pose relative to where we started (world(0, 0, 0) = where we started)
    float gyroZ;  // gyro z value in rad/s
    int plan;  // plan enum
    int direction; // direction sent from bluetooth
};

#endif // BLACKBOARD_H