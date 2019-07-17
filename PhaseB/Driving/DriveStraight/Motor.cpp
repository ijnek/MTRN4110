#include "Motor.hpp"
#include "Arduino.h"

Motor::Motor(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA, uint8_t motorDirB, bool isLeftMotor)
    : directionIsForwards(true),
      speed(0),
      counter(0),
      encoderPinA(encoderPinA),
      encoderPinB(encoderPinB),
      motorEn(motorEn),
      motorDirA(motorDirA),
      motorDirB(motorDirB),
      isLeftMotor(isLeftMotor)
{
    // setup encoder
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), speedInterrupt, CHANGE);
    
    // setup motor
    pinMode(motorEn, OUTPUT);
    pinMode(motorDirA, OUTPUT);
}

void Motor::setSpeedByPercentage(float speed) // set speed by percentage of 0-1
{
    setSpeedByUint8_t(255 * speed);
}

void Motor::setSpeedByUint8_t(uint8_t speed) // set speed directly by uint8_t
{
    analogWrite(motorEn, speed);
    Serial.println(speed);
}

void Motor::setDirectionToForwards(bool forwards) // set direction to forwards, or backwards
{
    if (isLeftMotor)
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
