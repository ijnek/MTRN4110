#ifndef PLAN_3_H
#define PLAN_3_H

#include "WayPointPlan.h"


class Plan3 : public WayPointPlan
{
  public:
    Plan3()
    {
        addWayPoint(WayPoint(1500, 0));
    }
};

#endif // PLAN_3_H