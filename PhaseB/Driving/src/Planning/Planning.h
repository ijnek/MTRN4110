#ifndef PLANNING_H
#define PLANNING_H

#include "../Blackboard/Blackboard.h"
#include "../MovementRequest/MovementRequest.h"
#include "Arduino.h"

class Planning
{
  public:
    Planning(Blackboard &blackboard)
      : blackboard(blackboard)
    {
    }

    void tick()
    {
        float forwardVelocity = 200;  // mm/s
        float turnVelocity = 0; // rad/s
        blackboard.movementRequest = MovementRequest(forwardVelocity, turnVelocity);
    }
  private:
    Blackboard &blackboard;
};


#endif // PLANNING_H