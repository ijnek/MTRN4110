#include "WayPointBehaviour.h"
#include "../Utils/MathUtil/MathUtil.h"
#include "../Constants/MazeConstants.h"


// Definitions of close when going to waypoints
#define CLOSE_DIST 50  // mm
#define CLOSE_DIST_SQUARED CLOSE_DIST*CLOSE_DIST  // mm^2
#define NOT_CLOSE_DIST 30  // mm
#define NOT_CLOSE_DIST_SQUARED CLOSE_DIST*CLOSE_DIST  // mm^2
#define SIDE_WAYS_ERROR_CLOSE 30  // mm
#define SIDE_WAYS_ERROR_NOT_CLOSE 50  // mm

#define MIN_STAY_TICKS 2  // how many ticks to wait for once arriving at a waypoint

WayPointBehaviour::WayPointBehaviour()
  : wayPoints(wayPointsArray)
{
    reset();
}

void WayPointBehaviour::reset()
{
    wayPointIndex = 0;
    headingClose = false;
    distanceClose = false;
    arrivedCount = 0;
    closeToFinalPoseCount = 0;
}

void WayPointBehaviour::addWayPoint(WayPoint wayPoint)
{
    wayPoints.push_back(wayPoint);
}

void WayPointBehaviour::clearWayPoints()
{
    wayPoints.clear();
}

bool WayPointBehaviour::done()  // whether the way point is done or not
{
    return wayPointIndex == wayPoints.size();
}


MovementRequest WayPointBehaviour::getMovementRequest(float myX, float myY, float myH)
{
    if (wayPointIndex == wayPoints.size())
    {
        return MovementRequest(0, 0);
    }

    float aimX = wayPoints[wayPointIndex].x;
    float aimY = wayPoints[wayPointIndex].y;

    float forwardAmount;  // mm
    float turnAmount;  // rad

    if (distanceIsClose(myX, myY, aimX, aimY))
    {
        closeToFinalPoseCount++;
        turnAmount = 0;
        forwardAmount = ((aimX - myX) * cos(myH) + (aimY - myY) * sin(myH) / 2);
        distanceClose = true;
    }
    else
    {
        if (headingIsClose(myX, myY, myH, aimX, aimY))
        {
            // If our heading is correct, then just go forwards
            // forwardAmount = sqrtf(distanceErrorSquared(myX, myY, aimX, aimY));
            forwardAmount = ((aimX - myX) * cos(myH) + (aimY - myY) * sin(myH)) / 2;
            turnAmount = 0;
            headingClose = true;
        }
        else
        {
            // If our heading is not close, then correct our heading first
            forwardAmount = 0;
            turnAmount = headingError(myX, myY, myH, aimX, aimY) / 2;
            headingClose = false;
        }
        distanceClose = false;
    }

    // If we're close to the current way point, let's go to next one!
    if (closeToFinalPoseCount > MIN_STAY_TICKS)
    {
        wayPointIndex = wayPointIndex + 1;
        closeToFinalPoseCount = 0;
    }
    
    return MovementRequest(forwardAmount, turnAmount);
}


float WayPointBehaviour::distanceErrorSquared(float currentX, float currentY, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return diffX * diffX + diffY * diffY;
}

bool WayPointBehaviour::distanceIsClose(float currentX, float currentY, float aimX, float aimY)
{
    if (distanceClose)
    {
        return distanceErrorSquared(currentX, currentY, aimX, aimY) < NOT_CLOSE_DIST_SQUARED;
    }
    else
    {
        return distanceErrorSquared(currentX, currentY, aimX, aimY) < CLOSE_DIST_SQUARED;
    }
}

float WayPointBehaviour::headingError(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return normaliseTheta(atan2(diffY, diffX) - currentH);
}

bool WayPointBehaviour::headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    // Have hysteresis to prevent flickering
    float distanceError = min(CELL_SIZE, sqrt(distanceErrorSquared(currentX, currentY, aimX, aimY)));  // no bigger than one cell
    float absHeadingError = abs(headingError(currentX, currentY, currentH, aimX, aimY));
    float sideWaysError = distanceError * sin(absHeadingError);

    if (headingClose)
    {
        return sideWaysError < SIDE_WAYS_ERROR_NOT_CLOSE;
    }
    else
    {
        return sideWaysError < SIDE_WAYS_ERROR_CLOSE;
    }
}
