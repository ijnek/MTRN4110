#include "Communication.h"
#include "Arduino.h"

void Communication::tick()
{
    while (!Serial1.available())
    {
        delay(100);
    }

    if (Serial1.available())
    {
        String s = Serial1.readString();
        blackboard.encodedMaze = s;
        // Serial.println(blackboard.encodedMaze);
        delay(10*1000);
    }
}