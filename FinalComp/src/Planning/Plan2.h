#ifndef PLAN_2_H
#define PLAN_2_H

#include "WayPointPlan.h"
#include "../MathUtil/MathUtil.h"

class Plan2 : public WayPointPlan
{
  public:
    Plan2()
    {
        addWayPoint(WayPoint(0, 0, DEG2RAD(0)));
    }
};

#endif // PLAN_2_H