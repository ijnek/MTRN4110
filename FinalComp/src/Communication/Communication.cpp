#include "Communication.h"
#include "Arduino.h"

void Communication::tick()
{
    // Serial.println("here");
    if (Serial1.available())
    {
        String s = Serial1.readString();
        blackboard.encodedMaze = s;
    }
}