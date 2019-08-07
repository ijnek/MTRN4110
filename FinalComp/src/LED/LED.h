#ifndef LED_H
#define LED_H

#include "../Constants/PinDefs.h"
#include "../Types/Blackboard.h"

class LED
{
  public:
    LED(Blackboard &blackboard) : blackboard(blackboard), firstTime(true), previousStartDetected(false), previousReachedGoal(false)
    {
        
    }

    void tick()
    {
        if (firstTime)
        {
            pinMode(LED_GREEN_DIGITAL_PIN, OUTPUT);
            pinMode(LED_RED_DIGITAL_PIN, OUTPUT);
            digitalWrite(LED_GREEN_DIGITAL_PIN, LOW);
            digitalWrite(LED_RED_DIGITAL_PIN, HIGH);
            firstTime = false;
        }

        if (!previousStartDetected)
        {
            if (blackboard.startDetected)
            {
                digitalWrite(LED_GREEN_DIGITAL_PIN, HIGH);
                digitalWrite(LED_RED_DIGITAL_PIN, LOW);
            }
        }
        
        if (!previousReachedGoal)
        {
            if (blackboard.reachedGoal)
            {
                digitalWrite(LED_GREEN_DIGITAL_PIN, LOW);
                digitalWrite(LED_RED_DIGITAL_PIN, HIGH);
            }
        }

        previousStartDetected = blackboard.startDetected;
        previousReachedGoal = blackboard.reachedGoal;
    }

  private:
    Blackboard &blackboard;
    bool firstTime;
    bool previousStartDetected;
    bool previousReachedGoal;
};

#endif // LED_H
