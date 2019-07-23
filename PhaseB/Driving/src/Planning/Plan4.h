#ifndef PLAN_4_H
#define PLAN_4_H

#include "WayPointPlan.h"

class Plan4 : public WayPointPlan
{
  public:
    Plan4()
    {
        addWayPoint(WayPoint(250, 0));
        addWayPoint(WayPoint(250, 250));
        addWayPoint(WayPoint(0, 250));
        addWayPoint(WayPoint(0, 500));
        addWayPoint(WayPoint(250, 500));
        addWayPoint(WayPoint(250, 750));
        addWayPoint(WayPoint(0, 750));
        addWayPoint(WayPoint(0, 1000));
        addWayPoint(WayPoint(250, 1000));
    }
};

#endif // PLAN_4_H