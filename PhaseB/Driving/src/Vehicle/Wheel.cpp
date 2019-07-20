#include "Wheel.h"
#include <Arduino.h>
#include "../Constants/Constants.h"

Wheel::Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA,
             uint8_t motorDirB, bool isLeftWheel, void (*f)())
    : encoderDetectingForwardsRotation(true),
      speed(0),
      encoderForwardsCounter(0),
      counterForOdometry(0),
      encoderPinA(encoderPinA),
      encoderPinB(encoderPinB),
      motorEn(motorEn),
      motorDirA(motorDirA),
      motorDirB(motorDirB),
      isLeftWheel(isLeftWheel),
      prevEncoderPinA(0),
      duration(0),
      absDuration(0),
      output(0),
      setPoint(0),
      pid(&absDuration, &output, &setPoint, MOTOR_K_P, MOTOR_K_I, MOTOR_K_D, DIRECT)
{
    // Setup encoder
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), f, CHANGE);

    // Setup motor
    pinMode(motorEn, OUTPUT);
    pinMode(motorDirA, OUTPUT);

    // Setup pid controller
    pid.SetMode(AUTOMATIC); // PID is set to automatic mode
    pid.SetSampleTime(100); // Set PID sampling frequency is 100ms
}

void Wheel::tick()
{
    setPoint = 100;
    absDuration = abs(duration);
    bool result = pid.Compute(); //PID conversion is complete and returns 1
    if (result)
    {
        // Serial.print("output: ");
        // Serial.println(output);
        // Serial.print("Pulse: ");
        // Serial.println(duration);
        duration = 0; //Count clear, wait for the next count
        writeSpeedByUint8_t(output);
    }

}

void Wheel::writeSpeedByPercentage(float speed) // set speed by percentage of 0-100%
{
    writeSpeedByUint8_t(255 * (speed / 100.0));
}

void Wheel::writeSpeedByUint8_t(uint8_t speed) // set speed directly by uint8_t
{
    analogWrite(motorEn, speed);
}

void Wheel::setSpeed(float speed) // set speed by how much wheel should travel in a second (mm/s)
{
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

int Wheel::getEncoderForwardsCounter()
{
    return encoderForwardsCounter;
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
    {
        encoderForwardsCounter++;
        duration++;
        counterForOdometry++;
    }
    else
    {
        encoderForwardsCounter--;
        duration--;
        counterForOdometry--;
    }
}

void Wheel::resetCounterForOdometry()
{
    counterForOdometry = 0;
}

int Wheel::getCounterForOdometry()
{
    return counterForOdometry;
}