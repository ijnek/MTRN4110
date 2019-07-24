#ifndef PLANNING_H
#define PLANNING_H

#include "../Blackboard/Blackboard.h"
#include "Plan.h"

#define MAX_PLAN_COUNT 30

class Planning
{
  public:
    Planning(Blackboard &blackboard);
    void tick();

  private:
    Blackboard &blackboard;
    Plan *plansArray[MAX_PLAN_COUNT];  // Allocate some memory to use the waypoints vector
    Vector<Plan*> plans;
    int prevPlan;  // Previous plan from last tick
};


#endif // PLANNING_H