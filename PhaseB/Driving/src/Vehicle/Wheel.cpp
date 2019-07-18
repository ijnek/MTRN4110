#include "Wheel.h"
#include <Arduino.h>

Wheel::Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA,
             uint8_t motorDirB, bool isLeftWheel, void (*f)())
    : encoderDetectingForwardsRotation(true),
      speed(0),
      encoderForwardsCounter(0),
      encoderPinA(encoderPinA),
      encoderPinB(encoderPinB),
      motorEn(motorEn),
      motorDirA(motorDirA),
      motorDirB(motorDirB),
      isLeftWheel(isLeftWheel),
      prevEncoderPinA(0)
{
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), f, CHANGE);
    pinMode(motorEn, OUTPUT);
    pinMode(motorDirA, OUTPUT);
}

void Wheel::setSpeedByPercentage(float speed) // set speed by percentage of 0-100%
{
    setSpeedByUint8_t(255 * (speed / 100.0));
}

void Wheel::setSpeedByUint8_t(uint8_t speed) // set speed directly by uint8_t
{
    analogWrite(motorEn, speed);
}

void Wheel::setDirectionToForwards(bool forwards)
{
    if (isLeftWheel)
    {
        if (forwards)
        {
            digitalWrite(motorDirA, HIGH);
            digitalWrite(motorDirB, LOW);
        }
        else
        {
            digitalWrite(motorDirA, LOW);
            digitalWrite(motorDirB, HIGH);
        }
    }
    else
    {
        if (forwards)
        {
            digitalWrite(motorDirA, LOW);
            digitalWrite(motorDirB, HIGH);
        }
        else
        {
            digitalWrite(motorDirA, HIGH);
            digitalWrite(motorDirB, LOW);
        }
    }
}

/*
 * Interrupt service for the wheel encoder
 */
void Wheel::encoderInterrupt()
{
    // Get the most recent state of the encoder
    int curr_state = digitalRead(encoderPinA);

    // Figure out which way the encoder is rotating if the state has changed
    if (prevEncoderPinA == LOW && curr_state == HIGH)
    {
        int val = digitalRead(encoderPinB);
        if (val == LOW && encoderDetectingForwardsRotation)
        {
            encoderDetectingForwardsRotation = false;
        }
        else if (val == HIGH && !encoderDetectingForwardsRotation)
        {
            encoderDetectingForwardsRotation = true;
        }
    }

    prevEncoderPinA = curr_state;

    // Increment/Decrementu the count
    if (encoderDetectingForwardsRotation)
        encoderForwardsCounter++;
    else
        encoderForwardsCounter--;
}
