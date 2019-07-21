#include "Wheel.h"
#include <Arduino.h>
#include "../Constants/Constants.h"
#include "../MathUtil/MathUtil.h"

Wheel::Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA,
             uint8_t motorDirB, bool isLeftWheel, void (*f)())
    : encoderDetectingForwardsRotation(true),
      counterForOdometry(0),
      encoderPinA(encoderPinA),
      encoderPinB(encoderPinB),
      motorEn(motorEn),
      motorDirA(motorDirA),
      motorDirB(motorDirB),
      isLeftWheel(isLeftWheel),
      prevEncoderPinA(0),
      counts(0),
      output(0),
      setPointCounts(0),
      pid(&counts, &output, &setPointCounts, MOTOR_K_P, MOTOR_K_I, MOTOR_K_D, DIRECT)
{
    // Setup encoder
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), f, CHANGE);

    // Setup motor
    pinMode(motorEn, OUTPUT);
    pinMode(motorDirA, OUTPUT);

    // Setup pid controller
    pid.SetMode(AUTOMATIC); // PID is set to automatic mode
    pid.SetSampleTime(S_TO_MS(PID_SAMPLE_TIME_S)); // Set PID sampling frequency is 100ms
    pid.SetOutputLimits(70, 200);
}

void Wheel::tick()
{
    pid.SetControllerDirection(setPointCounts < counts);
    bool result = pid.Compute(); //PID conversion is complete and returns 1
    if (result)
    {
        setDirectionToForwards(counts > setPointCounts ? DIRECT : REVERSE);
        // Serial.print("aim: ");
        // Serial.print(setPointCounts);
        // Serial.print(", counts: ");
        // Serial.print(counts);
        // Serial.print(", output: ");
        // Serial.println(output);
        counts = 0; //Count clear, wait for the next count
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

void Wheel::setAngularPosition(float angularPosition) // set angular position of wheel (rad)
{
    angularPosition = CLAMP(-DEG2RAD(500), angularPosition, DEG2RAD(500));
    setPointCounts = angularPosition * ENCODER_COUNTS_PER_REV / RAD_PER_REV;
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
    {
        if (isLeftWheel)
        {
            counts++;
            counterForOdometry++;
        }
        else
        {
            counts--;
            counterForOdometry--;
        }
    }
    else
    {
        if (isLeftWheel)
        {
            counts--;
            counterForOdometry--;
        }
        else
        {
            counts++;
            counterForOdometry++;
        }
    }
}

int Wheel::getAndResetCounterForOdometry()
{
    // important to reset straight after reading it as below. 
    // Since interrupts are constantly coming in, if we reset 
    // it later after reading it, it is possible miss some enocoder counts
    int c = counterForOdometry;
    counterForOdometry = 0;
    return c;
}