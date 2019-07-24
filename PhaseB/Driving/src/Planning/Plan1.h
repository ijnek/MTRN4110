#ifndef PLAN_1_H
#define PLAN_1_H

#include "WayPointPlan.h"

class Plan1 : public WayPointPlan
{
  public:
    Plan1()
    {
        addWayPoint(WayPoint(250, 0, 0));
    }
};

#endif // PLAN_1_H