#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <stdint.h>

class Wheel
{
  public:
    Wheel(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA, uint8_t motorDirB, bool isLeftWheel, void (*f)());
    void setSpeedByPercentage(float speed);  // set speed by percentage of 0-100%
    void setSpeedByUint8_t(uint8_t speed); // set speed directly by uint8_t
    void setDirectionToForwards(bool forwards); 
    void encoderInterrupt();

  private:
    bool encoderDetectingClockwiseRotation;
    uint8_t speed;
    int clockwiseCounter;

    // addresses to encoder and motor pins on arduino board
    uint8_t encoderPinA;
    uint8_t encoderPinB;
    uint8_t motorEn;
    uint8_t motorDirA;
    uint8_t motorDirB;

    // whether motor is the left wheel or right wheel
    bool isLeftWheel;

    int prevEncoderPinA;
};

#endif // WHEEL_HPP