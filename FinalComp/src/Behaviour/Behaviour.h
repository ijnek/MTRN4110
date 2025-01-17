#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "../Types/Blackboard.h"
#include "BehaviourPlan.h"

#define MAX_BEHAVIOUR_COUNT 30

class Behaviour
{
  public:
    Behaviour(Blackboard &blackboard);
    void tick();
    void reset();
    void addPlans();

  private:
    Blackboard &blackboard;
    BehaviourPlan *plansArray[MAX_BEHAVIOUR_COUNT];  // Allocate some memory to use the waypoints vector
    Vector<BehaviourPlan*> plans;
    int prevBehaviour;  // Previous plan from last tick
};


#endif // BEHAVIOUR_H