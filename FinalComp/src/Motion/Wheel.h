#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <stdint.h>
#include <Arduino.h>
#include "PID/PID.h"
#include "../Types/Odometry.h"

class Wheel
{
  public:
    Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA, uint8_t motorDirB, bool isLeftWheel, void (*f)());
    void tick();
    void writeSpeedByPercentage(float speed);  // set speed by percentage of 0-100%
    void writeSpeedByUint8_t(uint8_t speed); // set speed directly by uint8_t
    void setDirectionToForwards(bool forwards); 
    void encoderInterrupt();
    int getAndResetCounterForOdometry();
    void setAngularPosition(float angularPosition);  // set angular position, from start of run (rad)
    void reset()
    {
      counterForOdometry = 0;
      counts = 0;
      setPointCounts = 0;
      lastTime = millis();
    }

private:
    bool encoderDetectingForwardsRotation;
    int counterForOdometry;

    // addresses to encoder and motor pins on arduino board
    uint8_t encoderPinA;
    uint8_t encoderPinB;
    uint8_t motorEn;
    uint8_t motorDirA;
    uint8_t motorDirB;

    // whether motor is the left wheel or right wheel
    bool isLeftWheel;

    int prevEncoderPinA;

    // pid controller values
    double counts; //the number of the pulses detected since last cleared
    double setPointCounts;  // aim for counts per second
    PID pid;
    unsigned long lastTime;  // last time pid value was updated (ms)
};

#endif // WHEEL_HPP