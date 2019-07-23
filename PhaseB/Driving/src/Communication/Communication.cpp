#include "Communication.h"
#include "Arduino.h"

void Communication::tick()
{
    if (Serial1.available())
    {
        String s = Serial1.readString();
        Serial.println(s);

        if (s[0] == 'P')
        {
            // Its a plan!
            blackboard.plan = int(s[1]);
        }

        if (s[0] == 'D')
        {
            // Its a direction!
            blackboard.direction = int(s[1]);  // 1 for left, -1 for right
        }
    }
    else
    {
        blackboard.plan = 4;
        blackboard.direction = 0;
    }
}