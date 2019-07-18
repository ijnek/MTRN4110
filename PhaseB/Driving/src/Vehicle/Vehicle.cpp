#include "Vehicle.h"
#include "Wheel.h"
#include "../PinDefs/PinDefs.h"

// Instances of the wheel, had to do it this way because of the static interrupt functions
Wheel wheelL(ENCODER_PIN_A_L, ENCODER_PIN_B_L, MOTOR_EN_L,
             MOTOR_DIR_A_L, MOTOR_DIR_B_L, true, &leftEncoderInterrupt);
Wheel wheelR(ENCODER_PIN_A_R, ENCODER_PIN_B_R, MOTOR_EN_R,
             MOTOR_DIR_A_R, MOTOR_DIR_B_R, false, &rightEncoderInterrupt);

Vehicle::Vehicle()
{
}

void Vehicle::tick()
{
}

void Vehicle::addCommand(DriveCommand driveCommand)
{
    commandQueue.push_back(driveCommand);
}

static void leftEncoderInterrupt()
{
    wheelL.encoderInterrupt();
}


static void rightEncoderInterrupt()
{
    wheelR.encoderInterrupt();
}
