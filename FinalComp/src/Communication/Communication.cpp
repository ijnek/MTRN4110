#include "Communication.h"
#include "Arduino.h"

void Communication::tick()
{
    if (Serial1.available())
    {
        String string = Serial1.readString();
        if (string[0] == 'P')  // Behaviour
        {
            blackboard.plan = (int)string[1] - 48;   // convert ascii to number
            // blackboard.resetLocalisation = true;
        }
        else if (string == "90")
        {
            blackboard.plan = BEHAVIOUR_TURN_LEFT;
        }
        else if (string == "-90")
        {
            blackboard.plan = BEHAVIOUR_TURN_RIGHT;
        }
        else if (string == "1")
        {
            blackboard.nextMoveCommandReceived = true;
        }
    }
}