#include "Planning.h"
#include "../Constants/Constants.h"

void Planning::tick()
{
    if (fabs(blackboard.worldPose.theta) < DEG2RAD(90))
    {
        Serial.println("turn!");
        float forwardVelocity = 0; // mm/s
        float turnVelocity = DEG2RAD(10);      // rad/s
        blackboard.movementRequest = MovementRequest(forwardVelocity, turnVelocity);
    }
    else
    {
        Serial.println("stop!");
        blackboard.movementRequest = MovementRequest(0, 0);
    }
}