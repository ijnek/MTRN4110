#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <stdint.h>
#include "../PID/PID_v1.h"
#include "../Odometry/Odometry.h"

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

private:
    bool encoderDetectingForwardsRotation;
    uint8_t speed;
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
    double output;
    double setPointCounts;  // aim for counts per second
    PID pid;
};

#endif // WHEEL_HPP