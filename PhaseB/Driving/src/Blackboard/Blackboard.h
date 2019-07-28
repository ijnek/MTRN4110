#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../Odometry/Odometry.h"
#include "../MovementRequest/MovementRequest.h"
#include "../Vector/Vector.h"
#include "../Pose/Pose.h"
#include "../Constants/Constants.h"
#include "../WayPoint/WayPoint.h"

class Blackboard
{
  public:
    Blackboard(): gyroZ(0), plan(PLAN_NONE), resetLocalisation(false), wallInFront(false), wallOnLeft(false), wallOnRight(false), commandCompleted(false), startDetected(false), nextMoveCommandReceived(false){}

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
    WayPoint nextWayPoint;  // next way point to go to
    bool reachedGoal;  // whether we have reached the goal or not   
    bool startDetected;  // whether we have detected the start signal 
    bool nextMoveCommandReceived;  // whether we have received the next move command through bluetooth
};

#endif // BLACKBOARD_H