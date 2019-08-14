#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "Odometry.h"
#include "MovementRequest.h"
#include "../Utils/Vector/Vector.h"
#include "Pose.h"
#include "../Constants/NetworkingConstants.h"
#include "WayPoint.h"

class Blackboard
{
  public:
    Blackboard(): gyroZ(0), plan(BEHAVIOUR_NONE), resetLocalisation(false), wallInFront(false), wallOnLeft(false), wallOnRight(false), commandCompleted(false), startDetected(false), nextMoveCommandReceived(false), plannedWayPoints(plannedWayPointsArray){}

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
    float lidarLeft;  // distance of front lidar obstacle
    float lidarFront;  // distance of left lidar obstacle
    float lidarRight;  // distance of right lidar obstacle
    String encodedMaze;  // string that gets transmitted from vision or exploration to planning
    WayPoint plannedWayPointsArray[15];  // Allocate some memory to use the waypoints vector
    Vector<WayPoint> plannedWayPoints;  // planned way points from planning
};

#endif // BLACKBOARD_H