#ifndef WAY_POINT_BEHAVIOUR_H
#define WAY_POINT_BEHAVIOUR_H

#include "../WayPoint/WayPoint.h"
#include "../Vector/Vector.h"
#include "../MovementRequest/MovementRequest.h"
#include "BehaviourPlan.h"

#define MAX_WAYPOINT_COUNT 30

class WayPointBehaviour : public BehaviourPlan
{
  public:
    WayPointBehaviour();
    void reset();
    void addWayPoint(WayPoint wayPoint);  // add a waypoint
    void clearWayPoints();
    bool done();  // whether the way point is done or not
    MovementRequest getMovementRequest(float myX, float myY, float myH);

  private:
    WayPoint wayPointsArray[MAX_WAYPOINT_COUNT];  // Allocate some memory to use the waypoints vector
    Vector<WayPoint> wayPoints;
    int wayPointIndex;
    bool headingClose;  // whether our heading is close, used for hysteresis purpose
    int closeToFinalPoseCount;  // whether our heading is close to final heading, used for hysteresis purpose
    int arrivedCount;

    float distanceErrorSquared(float currentX, float currentY, float aimX, float aimY);
    bool distanceIsClose(float currentX, float currentY, float aimX, float aimY);
    float headingError(float currentX, float currentY, float currentH, float aimX, float aimY);
    bool headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY);
    bool headingIsCloseToFinalHeading(float currentH, float finalHeading);
};

#endif // WAY_POINT_BEHAVIOUR_H