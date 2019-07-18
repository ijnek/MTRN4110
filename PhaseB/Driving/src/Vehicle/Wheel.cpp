#include "Wheel.h"
#include <Arduino.h>

Wheel::Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA,
            uint8_t motorDirB, bool isLeftWheel, void (*f)())
    : encoderDetectingClockwiseRotation(true),
      speed(0),
      clockwiseCounter(0),
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
 * Interrupt service for the left wheel encoder
 */
void Wheel::encoderInterrupt()
{
    // Get the most recent state of the encoder
    int curr_state = digitalRead(encoderPinA);

    // Figure out which way the encoder is rotating if the state has changed
    if (prevEncoderPinA == LOW && curr_state == HIGH)
    {
        int val = digitalRead(encoderPinB);
        Serial.print(curr_state);
        Serial.print(", ");
        Serial.println(val);
        if (val == LOW && encoderDetectingClockwiseRotation)
        {
            encoderDetectingClockwiseRotation = false;
            Serial.println(__LINE__);
        }
        else if (val == HIGH && !encoderDetectingClockwiseRotation)
        {
            encoderDetectingClockwiseRotation = true;
            Serial.println(__LINE__);
        }
    }
    prevEncoderPinA = curr_state;    

    // Increment/Decrement the count
    if (encoderDetectingClockwiseRotation)
        clockwiseCounter++;
    else
        clockwiseCounter--;

    Serial.print("curr_state: ");
    Serial.print(curr_state);
    Serial.print(", counter: ");
    Serial.print(clockwiseCounter);
    Serial.print("\n");
}