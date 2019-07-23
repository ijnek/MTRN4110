#ifndef PLAN_2_H
#define PLAN_2_H

#include "WayPointPlan.h"

class Plan2 : public WayPointPlan
{
  public:
    Plan2()
    {
        addWayPoint(WayPoint(0, 0));
        addWayPoint(WayPoint(250, 0));
    }
};

#endif // PLAN_2_H