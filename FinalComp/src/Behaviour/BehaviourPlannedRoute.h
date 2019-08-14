#ifndef BEHAVIOUR_PLANNED_ROUTE_H
#define BEHAVIOUR_PLANNED_ROUTE_H

#include "WayPointBehaviour.h"

class BehaviourPlannedRoute : public WayPointBehaviour
{
  public:
    BehaviourPlannedRoute()
    {
        addWayPoint(WayPoint(0, 0, 0));  // add a stub waypoint
    }

    void addWayPoints();

    void reset()
    {
        clearWayPoints();
        WayPointBehaviour::reset();
    }
};


#endif // BEAVIOUR_PLANNED_ROUTE_H