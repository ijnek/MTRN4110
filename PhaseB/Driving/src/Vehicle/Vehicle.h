#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "../Vector/Vector.h"

enum DriveCommand
{
    FORWARD,  // goes forward one square
    BACKWARD,  // goes backward one square
    TURN_LEFT,  // turns left 90 degrees
    TURN_RIGHT,  // turns right 90 degrees
};

class Vehicle
{
  public:
    Vehicle();
    void tick();
    void addCommand(DriveCommand driveCommand);

  private:
    Vector<DriveCommand> commandQueue;
};

static void leftEncoderInterrupt();
static void rightEncoderInterrupt();

#endif // VEHICLE_HPP