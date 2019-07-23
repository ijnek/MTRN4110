#include "Planning.h"
#include "../Constants/Constants.h"
#include "../MovementRequest/MovementRequest.h"
#include "../MathUtil/MathUtil.h"

Planning::Planning(Blackboard &blackboard)
    : blackboard(blackboard),
      plan1(Plan1()),
      plan2(Plan2()),
      plan3(Plan3()),
      plan4(Plan4())
{
}

void Planning::tick()
{

    float myX = blackboard.worldPose.x;
    float myY = blackboard.worldPose.y;
    float myH = blackboard.worldPose.theta;

    MovementRequest movementRequest;

    switch (blackboard.plan)
    {
        case PLAN1:
        {
            if (!plan1.done())
                movementRequest = plan1.getMovementRequest(myX, myY, myH);
            break;
        }
        case PLAN2:
        {
            if (!plan2.done())
                movementRequest = plan2.getMovementRequest(myX, myY, myH);
            break;
        }
        case PLAN3:
        {
            if (!plan3.done())
                movementRequest = plan3.getMovementRequest(myX, myY, myH);
            break;
        }
        case PLAN4:
        {
            if (!plan4.done())
                movementRequest = plan4.getMovementRequest(myX, myY, myH);
            break;
        }
    }

    blackboard.movementRequest = movementRequest;
}