#ifndef PLAN_TURN_RIGHT_H
#define PLAN_TURN_RIGHT_H

#include "Plan.h"
#include "../MathUtil/MathUtil.h"

#define MIN_ARRIVED_TICKS 20

class PlanTurnRight : public Plan
{
  public:

    void reset()
    {
        firstTime = true;
        arrived = false;
    }

    bool done()
    {
        return arrived;
    }

    MovementRequest getMovementRequest(float myX, float myY, float myH)
    {

        if (firstTime)
        {
            finalHeading = normaliseTheta(myH - DEG2RAD(90));
            firstTime = false;
        }

        if (fabs(normaliseTheta(finalHeading - myH)) < DEG2RAD(3))
        {
            arrivedCounter++;
        }
        else
        {
            arrivedCounter = 0;
        }

        if (arrivedCounter > MIN_ARRIVED_TICKS)
        {
            arrived = true;
        }
        else
        {
            arrived = false;
        }
        
        return MovementRequest(0, normaliseTheta(finalHeading - myH));
    }

  private:
    float finalHeading;
    bool firstTime;
    bool arrived;
    int arrivedCounter;
};

#endif // PLAN_TURN_RIGHT_H