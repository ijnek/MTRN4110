#ifndef WAY_POINT_PLAN_H
#define WAY_POINT_PLAN_H

#include "../WayPoint/WayPoint.h"
#include "../Vector/Vector.h"
#include "../MovementRequest/MovementRequest.h"

#define MAX_WAYPOINT_COUNT 30

class WayPointPlan
{
  public:
    WayPointPlan();
    void addWayPoint(WayPoint wayPoint);  // add a waypoint
    bool done();  // whether the way point is done or not
    MovementRequest getMovementRequest(float myX, float myY, float myH);

  private:
    WayPoint wayPointsArray[MAX_WAYPOINT_COUNT];  // Allocate some memory to use the waypoints vector
    Vector<WayPoint> wayPoints;
    int wayPointIndex;
    bool headingClose;  // whether our heading is close, used for hysteresis purpose
    int arrivedCount;

    float distanceErrorSquared(float currentX, float currentY, float aimX, float aimY);
    bool distanceIsClose(float currentX, float currentY, float aimX, float aimY);
    float headingError(float currentX, float currentY, float currentH, float aimX, float aimY);
    bool headingIsClose(float currentX, float currentY, float currentH, float aimX, float aimY);
};

#endif // WAY_POINT_PLAN_H