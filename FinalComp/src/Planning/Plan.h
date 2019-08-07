#ifndef PLAN_H
#define PLAN_H

#include "../MovementRequest/MovementRequest.h"

class Plan
{
  public:
    virtual void reset() = 0; // reset, in case plan is called the second time
    virtual bool done() = 0;  // whether the way point is done or not
    virtual MovementRequest getMovementRequest(float myX, float myY, float myH) = 0;
};

#endif // PLAN_H