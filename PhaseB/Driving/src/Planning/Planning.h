#ifndef PLANNING_H
#define PLANNING_H

#include "../Blackboard/Blackboard.h"
#include "../MovementRequest/MovementRequest.h"
#include "Arduino.h"

class Planning
{
  public:
    Planning(Blackboard &blackboard)
      : blackboard(blackboard) {}

    void tick();

  private:
    Blackboard &blackboard;
};


#endif // PLANNING_H