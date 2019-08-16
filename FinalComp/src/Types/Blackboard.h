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
    Blackboard(): 
      gyroZ(0),
      plan(BEHAVIOUR_NONE),
      resetRequested(false),
      wallInFront(false),
      wallOnLeft(false),
      wallOnRight(false),
      commandCompleted(false),
      startDetected(false),
      plannedWayPoints(plannedWayPointsArray),
      isExplorationRun(true),
      isVisionRun(false),
      pathPlanned(false){}

    void reset()
    {
        odometryDiff = Odometry();
        movementRequest = MovementRequest();
        worldPose = Pose(0, 0, 0);
        gyroZ = 0;
        plan = BEHAVIOUR_NONE;
        resetRequested = false;
        commandCompleted = false;
        wallInFront = false;
        wallOnLeft = false;
        wallOnRight = false;
        startDetected = false;
        nextWayPoint = WayPoint(0, 0, 0);
        reachedGoal = false;
        startDetected = false;
        lidarLeft = 255;
        lidarFront = 255;
        lidarRight = 255;
        plannedWayPoints.clear();
        pathPlanned = false;
    }

    Odometry odometryDiff;  // difference in odometry since last cycle
    MovementRequest movementRequest;  // current requested velocity
    Pose worldPose;  // pose relative to where we started (world(0, 0, 0) = where we started)
    float gyroZ;  // gyro z value in rad/s
    int plan;  // plan enum
    bool resetRequested;  // whether to do a run reset
    bool commandCompleted; // whether the command has been completed, by motion
    bool wallInFront;  // whether there is a wall in front
    bool wallOnLeft;  // whether there is a wall on left side
    bool wallOnRight;  // whether there is a wall on right side
    WayPoint nextWayPoint;  // next way point to go to
    bool reachedGoal;  // whether we have reached the goal or not   
    bool startDetected;  // whether we have detected the start signal 
    float lidarLeft;  // distance of front lidar obstacle
    float lidarFront;  // distance of left lidar obstacle
    float lidarRight;  // distance of right lidar obstacle
    String encodedMaze;  // string that gets transmitted from vision or exploration to planning
    WayPoint plannedWayPointsArray[30];  // Allocate some memory to use the waypoints vector
    Vector<WayPoint> plannedWayPoints;  // planned way points from planning
    bool isExplorationRun;  // whether the run is an exploration is an Exploration Run
    bool isVisionRun;  // whether the run is a vision run
    bool pathPlanned;  // whether we have planned the path
};

#endif // BLACKBOARD_H