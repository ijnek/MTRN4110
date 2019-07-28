#ifndef PLAN_NEXT_WAY_POINT_H
#define PLAN_NEXT_WAY_POINT_H

#include "WayPointPlan.h"

class PlanNextWayPoint : public WayPointPlan
{
  public:
    PlanNextWayPoint()
    {
    }

    void reset()
    {
        clearWayPoints();
        WayPointPlan::reset();
    }
};

#endif // PLAN_NEXT_WAY_POINT_H