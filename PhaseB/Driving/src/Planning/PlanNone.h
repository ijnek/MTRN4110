#ifndef PLAN_NONE_H
#define PLAN_NONE_H

#include "Plan.h"

class PlanNone : public Plan
{
  public:
    void reset() {}
    bool done() {return true;}  // always done
    MovementRequest getMovementRequest(float myX, float myY, float myH) {return MovementRequest();};  // return 0, 0 movement request
};

#endif // PLAN_NONE_H