#ifndef BEHAVIOUR_NONE_H
#define BEHAVIOUR_NONE_H

#include "BehaviourPlan.h"

class BehaviourNone : public BehaviourPlan
{
  public:
    void reset() {}
    bool done() {return true;}  // always done
    MovementRequest getMovementRequest(float myX, float myY, float myH) {return MovementRequest();};  // return 0, 0 movement request
};

#endif // BEHAVIOUR_NONE_H