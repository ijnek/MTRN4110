#include "Planning.h"
#include "../Constants/Constants.h"
#include "../MovementRequest/MovementRequest.h"
#include "../MathUtil/MathUtil.h"

// Allocate some memory to use the waypoints vector
#define MAX_WAYPOINT_COUNT 30
WayPoint waypointsArray[MAX_WAYPOINT_COUNT];

// Definitions of close when going to waypoints
#define CLOSE_DIST 50  // mm
#define CLOSE_DIST_SQUARED CLOSE_DIST*CLOSE_DIST  // mm^2
#define CLOSE_HEADING DEG2RAD(2)  // rad
#define NOT_CLOSE_HEADING DEG2RAD(5)  // rad


Planning::Planning(Blackboard &blackboard)
  : blackboard(blackboard),
    wayPoints(waypointsArray),
    wayPointIndex(0),
    headingClose(false)
{
    // Add some waypoints 

    // Task 1
    // wayPoints.push_back(WayPoint(0, 0));
    // wayPoints.push_back(WayPoint(250, 0));

    // // Task 2 (not done)
    // wayPoints.push_back(WayPoint(0, 0));

    // // Task 3
    // wayPoints.push_back(WayPoint(6*250, 0));

    // Task 4
    wayPoints.push_back(WayPoint(250, 0));
    wayPoints.push_back(WayPoint(250, 250));
    wayPoints.push_back(WayPoint(0, 250));
    wayPoints.push_back(WayPoint(0, 500));
    wayPoints.push_back(WayPoint(250, 500));
    wayPoints.push_back(WayPoint(250, 750));
    wayPoints.push_back(WayPoint(0, 750));
    wayPoints.push_back(WayPoint(0, 1000)); 
    wayPoints.push_back(WayPoint(250, 1000));   
}

void Planning::tick()
{

    float myX = blackboard.worldPose.x;
    float myY = blackboard.worldPose.y;
    float myH = blackboard.worldPose.theta;
    float aimX = wayPoints[wayPointIndex].x;
    float aimY = wayPoints[wayPointIndex].y;

    float forwardAmount;  // mm
    float turnAmount;  // rad

    // If we're close to the current way point, let's go to next one!
    if (distanceIsClose(myX, myY, aimX, aimY))
    {
        wayPointIndex = (wayPointIndex + 1) % wayPoints.size();
        // Serial.print("next way point: waypoint");
        // Serial.print(wayPointIndex);
        // Serial.print(" - ");
        // Serial.print(wayPoints[wayPointIndex].x);
        // Serial.print(", ");
        // Serial.println(wayPoints[wayPointIndex].y);
    }

    float headingErr = headingError(myX, myY, myH, aimX, aimY);
    float distanceErrSquared = distanceErrorSquared(myX, myY, aimX, aimY);
    
    // If our heading is not close, then correct our heading first
    if (!headingIsClose(myX, myY, myH, aimX, aimY))
    {
        forwardAmount = 0;
        turnAmount = headingErr;
        headingClose = false;
    }
    else
    {
        forwardAmount = sqrtf(distanceErrSquared);
        turnAmount = headingErr;  // try and adjust by something
        headingClose = true;
    }

    blackboard.movementRequest = MovementRequest(forwardAmount, turnAmount);
}

float Planning::distanceErrorSquared(float currentX, float currentY, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return diffX * diffX + diffY * diffY;
}

bool Planning::distanceIsClose(float currentX, float currentY, float aimX, float aimY)
{
    return distanceErrorSquared(currentX, currentY, aimX, aimY) < CLOSE_DIST_SQUARED;
}

float Planning::headingError(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return normaliseTheta(atan2(diffY, diffX) - currentH);
}

bool Planning::headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    // Have hysteresis to prevent flickering
    if (headingClose)
    {
        return fabs(headingError(currentX, currentY, currentH, aimX, aimY)) < NOT_CLOSE_HEADING;
    }
    else
    {
        return fabs(headingError(currentX, currentY, currentH, aimX, aimY)) < CLOSE_HEADING;
    }
}
