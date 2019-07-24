#ifndef PLAN_FORWARDS_H
#define PLAN_FORWARDS_H

#include "WayPointPlan.h"
#include "../MathUtil/MathUtil.h"
#include "../Constants/Constants.h"

// THIS IS QUITE A HACKY IMPLEMENTATION TBH, but it works

class PlanForwards : public WayPointPlan
{
  public:

    void reset()
    {
        WayPointPlan::reset();
        firstTime = true;
        clearWayPoints();
        addWayPoint(WayPoint(0, 0, 0)); // add a dummy waypoint
    }

    MovementRequest getMovementRequest(float myX, float myY, float myH)
    {
        if (firstTime)
        {
            float finalX = myX + CELL_SIZE * cos(myH);
            float finalY = myY + CELL_SIZE * sin(myH);
            Serial.println(finalX);
            Serial.println(finalY);
            clearWayPoints();
            addWayPoint(WayPoint(finalX, finalY, myH));
            firstTime = false;
        }

        return WayPointPlan::getMovementRequest(myX, myY, myH);       
    }

  private:
    bool firstTime;

};

#endif // PLAN_FORWARDS_H