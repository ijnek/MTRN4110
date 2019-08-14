#include "Wheel.h"
#include <Arduino.h>
#include "../Constants/VehicleConstants.h"
#include "../Utils/MathUtil/MathUtil.h"


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
      setPointCounts(0),
      pid(MOTOR_K_P, MOTOR_K_I, MOTOR_K_D),
      lastTime(millis())
{
    // Setup encoder
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), f, CHANGE);

    // Setup motor
    pinMode(motorEn, OUTPUT);
    pinMode(motorDirA, OUTPUT);
}

void Wheel::tick()
{
    // If we've reached sampling period, update pid
    unsigned long time = millis();
    if (time - lastTime > PID_SAMPLING_PERIOD_MS)
    {
        int result = pid.compute(setPointCounts, counts);
        counts = 0; //Count clear, wait for the next count
        setDirectionToForwards(result > 0 ? true : false);  // set the directon of the motor

        // Modify value by multiplying it, to achieve similar results in left and right motor
        if (isLeftWheel)
            result *= LEFT_MOTOR_PWM_MULTIPLIER;
        else
            result *= RIGHT_MOTOR_PWM_MULTIPLIER;
        writeSpeedByUint8_t(abs(result));

        lastTime = time; // update lasttime pid was updated
    }
}

void Wheel::writeSpeedByUint8_t(uint8_t speed) // set speed directly by uint8_t
{
    analogWrite(motorEn, speed);
}

void Wheel::setAngularPosition(float angularPosition) // set angular position of wheel (rad)
{
    setPointCounts = angularPosition * ENCODER_COUNTS_PER_REV / RAD_PER_REV;
}

void Wheel::setDirectionToForwards(bool forwards)
{
    if (isLeftWheel)
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
        if (isLeftWheel)
        {
            if (val == LOW && encoderDetectingForwardsRotation)
            {
                encoderDetectingForwardsRotation = false;
            }
            else if (val == HIGH && !encoderDetectingForwardsRotation)
            {
                encoderDetectingForwardsRotation = true;
            }
        }
        else
        {
            if (val == LOW && !encoderDetectingForwardsRotation)
            {
                encoderDetectingForwardsRotation = true;
            }
            else if (val == HIGH && encoderDetectingForwardsRotation)
            {
                encoderDetectingForwardsRotation = false;
            }            
        }
    }

    prevEncoderPinA = curr_state;

    // Increment/Decrementu the count
    if (encoderDetectingForwardsRotation)
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

int Wheel::getAndResetCounterForOdometry()
{
    // important to reset straight after reading it as below. 
    // Since interrupts are constantly coming in, if we reset 
    // it later after reading it, it is possible miss some enocoder counts
    int c = counterForOdometry;
    counterForOdometry = 0;
    return c;
}