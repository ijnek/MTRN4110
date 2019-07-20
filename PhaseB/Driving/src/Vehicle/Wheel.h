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
    void setSpeed(float speed);  // set speed by how much wheel should travel in a second (mm/s)
    void setDirectionToForwards(bool forwards); 
    void encoderInterrupt();
    int getEncoderForwardsCounter();
    void resetCounterForOdometry();
    int getCounterForOdometry();

  private:
    bool encoderDetectingForwardsRotation;
    uint8_t speed;
    int encoderForwardsCounter;
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
    double duration, absDuration; //the number of the pulses detected since last cleared
    double output;
    double setPoint;
    PID pid;
};

#endif // WHEEL_HPP