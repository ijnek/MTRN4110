#ifndef PLAN_4_H
#define PLAN_4_H

#include "WayPointPlan.h"
#include "../MathUtil/MathUtil.h"

class Plan4 : public WayPointPlan
{
  public:
    Plan4()
    {
        addWayPoint(WayPoint(250, 0, DEG2RAD(90)));
        addWayPoint(WayPoint(250, 250, DEG2RAD(180)));
        addWayPoint(WayPoint(0, 250, DEG2RAD(90)));
        addWayPoint(WayPoint(0, 500, 0));
        addWayPoint(WayPoint(250, 500, DEG2RAD(90)));
        addWayPoint(WayPoint(250, 750, DEG2RAD(180)));
        addWayPoint(WayPoint(0, 750, DEG2RAD(90)));
        addWayPoint(WayPoint(0, 1000, 0));
        addWayPoint(WayPoint(250, 1000, 0));
    }
};

#endif // PLAN_4_H