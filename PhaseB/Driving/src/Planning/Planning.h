#ifndef PLANNING_H
#define PLANNING_H

#include "../Blackboard/Blackboard.h"
#include "Plan1.h"
#include "Plan2.h"
#include "Plan3.h"
#include "Plan4.h"

class Planning
{
  public:
    Planning(Blackboard &blackboard);
    void tick();

  private:
    Blackboard &blackboard;

    Plan1 plan1;
    Plan2 plan2;
    Plan3 plan3;
    Plan4 plan4;
};


#endif // PLANNING_H