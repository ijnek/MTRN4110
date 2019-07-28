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
    Blackboard(): gyroZ(0), plan(PLAN_NONE), resetLocalisation(false), commandCompleted(false){}

    Odometry odometryDiff;  // difference in odometry since last cycle
    MovementRequest movementRequest;  // current requested velocity
    Pose worldPose;  // pose relative to where we started (world(0, 0, 0) = where we started)
    float gyroZ;  // gyro z value in rad/s
    int plan;  // plan enum
    int command; // command going to motion
    bool resetLocalisation;
    bool commandCompleted; // whether the command has been completed, by motion
    bool wallInFront;  // whether there is a wall in front
    bool wallOnLeft;  // whether there is a wall on left side
    bool wallOnRight;  // whether there is a wall on right side
};

#endif // BLACKBOARD_H