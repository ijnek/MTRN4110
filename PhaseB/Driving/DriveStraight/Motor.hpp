#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>

class Motor
{
  public:
    Motor(uint8_t encoderPinA, uint8_t encoderPinB, uint8_t motorEn, uint8_t motorDirA, uint8_t motorDirB, bool isLeftMotor);
    void setSpeedByPercentage(float speed);  // set speed by percentage of 0-1
    void setSpeedByUint8_t(uint8_t speed); // set speed directly by uint8_t
    void setDirectionToForwards(bool forwards);    

  private:
    static void speedInterrupt(){}  // stub speed interrupt

    bool directionIsForwards;
    uint8_t speed;
    int counter;

    // addresses to encoder and motor pins on arduino board
    uint8_t encoderPinA;
    uint8_t encoderPinB;
    uint8_t motorEn;
    uint8_t motorDirA;
    uint8_t motorDirB;

    // whether motor is the left wheel or right wheel
    bool isLeftMotor;
};

#endif // MOTOR_HPP
