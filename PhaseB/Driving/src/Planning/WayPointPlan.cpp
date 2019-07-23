#include "WayPointPlan.h"
#include "../MathUtil/MathUtil.h"


// Definitions of close when going to waypoints
#define CLOSE_DIST 50  // mm
#define CLOSE_DIST_SQUARED CLOSE_DIST*CLOSE_DIST  // mm^2
#define CLOSE_HEADING DEG2RAD(2)  // rad
#define NOT_CLOSE_HEADING DEG2RAD(6)  // rad

#define WAIT_TICKS 20  // how many ticks to wait for once arriving at a waypoint

WayPointPlan::WayPointPlan()
  : wayPoints(wayPointsArray),
    wayPointIndex(0),
    headingClose(false),
    arrivedCount(0)
{
}

void WayPointPlan::addWayPoint(WayPoint wayPoint)
{
    wayPoints.push_back(wayPoint);
}

bool WayPointPlan::done()  // whether the way point is done or not
{
    return wayPointIndex == wayPoints.size();
}


MovementRequest WayPointPlan::getMovementRequest(float myX, float myY, float myH)
{
    if (wayPointIndex == wayPoints.size())
    {
        return MovementRequest(0, 0);
    }

    float aimX = wayPoints[wayPointIndex].x;
    float aimY = wayPoints[wayPointIndex].y;

    float forwardAmount;  // mm
    float turnAmount;  // rad

    // If we're close to the current way point, let's go to next one!
    if (distanceIsClose(myX, myY, aimX, aimY))
    {
        arrivedCount++;
        if (arrivedCount > WAIT_TICKS)
        {
            wayPointIndex = wayPointIndex + 1;
            arrivedCount = 0;
        }
    }
    else
    {
        arrivedCount = 0;
    }

    float headingErr = headingError(myX, myY, myH, aimX, aimY);
    float distanceErrSquared = distanceErrorSquared(myX, myY, aimX, aimY);
    
    if (headingIsClose(myX, myY, myH, aimX, aimY))
    {
        // If our heading is correct, then just go forwards
        forwardAmount = sqrtf(distanceErrSquared);
        turnAmount = 0;
        headingClose = true;
    }
    else
    {
        // If our heading is not close, then correct our heading first
        forwardAmount = 0;
        turnAmount = headingErr;
        headingClose = false;
    }
    
    return MovementRequest(forwardAmount, turnAmount);
}


float WayPointPlan::distanceErrorSquared(float currentX, float currentY, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return diffX * diffX + diffY * diffY;
}

bool WayPointPlan::distanceIsClose(float currentX, float currentY, float aimX, float aimY)
{
    return distanceErrorSquared(currentX, currentY, aimX, aimY) < CLOSE_DIST_SQUARED;
}

float WayPointPlan::headingError(float currentX, float currentY, float currentH, float aimX, float aimY)
{
    float diffX = aimX - currentX;
    float diffY = aimY - currentY;
    return normaliseTheta(atan2(diffY, diffX) - currentH);
}

bool WayPointPlan::headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY)
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
