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
#define CLOSE_HEADING DEG2RAD(10)  // rad


Planning::Planning(Blackboard &blackboard)
  : blackboard(blackboard),
    wayPoints(waypointsArray),
    wayPointIndex(0)
{
    // Add some waypoints
    wayPoints.push_back(WayPoint(0, 0));
    wayPoints.push_back(WayPoint(250, 0));
    wayPoints.push_back(WayPoint(250, 500));
    wayPoints.push_back(WayPoint(500, 500));
    wayPoints.push_back(WayPoint(500, 0));
}

void Planning::tick()
{

    float myX = blackboard.worldPose.x;
    float myY = blackboard.worldPose.y;
    float myH = blackboard.worldPose.theta;
    float aimX = wayPoints[wayPointIndex].x;
    float aimY = wayPoints[wayPointIndex].y;

    float forwardVelocity;  // mm / s
    float turnVelocity;  // rad / s

    // If we're close to the current way point, let's go to next one!
    if (distanceClose(myX, myY, aimX, aimY))
    {
        wayPointIndex = (wayPointIndex + 1) % wayPoints.size();
        Serial.print("next way point: waypoint");
        Serial.print(wayPointIndex);
        Serial.print(" - ");
        Serial.print(wayPoints[wayPointIndex].x);
        Serial.print(", ");
        Serial.println(wayPoints[wayPointIndex].y);
    }
    
    // If our heading is not close, then correct our heading first
    if (!headingClose(myX, myY, myH, aimX, aimY))
    {
        forwardVelocity = 0;
        turnVelocity = headingError(myX, myY, myH, aimX, aimY) / 1.0; // correct heading over 0.3 second (rad/s)
    }
    else
    {
        forwardVelocity = 500;  // (mm/s)
        turnVelocity = headingError(myX, myY, myH, aimX, aimY) / 1.0;  // correct heading over 0.5 second (rad/s)
    }

    blackboard.movementRequest = MovementRequest(forwardVelocity, turnVelocity);
}

float Planning::distanceErrorSquared(float currentX, float currentY, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return diffX * diffX + diffY * diffY;
}

bool Planning::distanceClose(float currentX, float currentY, float aimX, float aimY)
{
    return distanceErrorSquared(currentX, currentY, aimX, aimY) < CLOSE_DIST_SQUARED;
}

float Planning::headingError(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return normaliseTheta(atan2(diffY, diffX) - currentH);
}

bool Planning::headingClose(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    return fabs(headingError(currentX, currentY, currentH, aimX, aimY)) < CLOSE_HEADING;
}
