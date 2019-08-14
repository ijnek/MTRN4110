#ifndef SENSING_H
#define SENSING_H

#include "IMU/IMU.h"
#include "../Types/Blackboard.h"
#include "Lidar/VL6180X.h"
#include "../Constants/PinDefs.h"

#define LIDAR_FRONT 0
#define LIDAR_LEFT 1
#define LIDAR_RIGHT 2
#define NUM_LIDARS 3

#define MAX_DISTNANCE_TO_CONSIDER_OBSTACLE 150  // mm

class Sensing
{
public:
  Sensing(Blackboard &blackboard) : blackboard(blackboard), firstTime(true)
  {
    lidars[LIDAR_FRONT] = VL6180X();
    lidars[LIDAR_LEFT] = VL6180X();
    lidars[LIDAR_RIGHT] = VL6180X();
  }

  void reset()
  {
    // Make some references, for ease of access
    VL6180X &lidarFront = lidars[LIDAR_FRONT];
    VL6180X &lidarLeft = lidars[LIDAR_LEFT];
    VL6180X &lidarRight = lidars[LIDAR_RIGHT];

    // set up enables
    pinMode(LIDAR_LEFT_SHDN, OUTPUT);
    pinMode(LIDAR_RIGHT_SHDN, OUTPUT);
    pinMode(LIDAR_FRONT_SHDN, OUTPUT);
    digitalWrite(LIDAR_LEFT_SHDN, LOW);
    digitalWrite(LIDAR_RIGHT_SHDN, LOW);
    digitalWrite(LIDAR_FRONT_SHDN, LOW);
    delay(1000);

    // set up first sensor
    // left lidar, connect to pin 22
    digitalWrite(LIDAR_LEFT_SHDN, HIGH);
    delay(50);
    lidarLeft.init();
    lidarLeft.configureDefault();
    lidarLeft.setAddress(0x30); // change to new address
    lidarLeft.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    lidarLeft.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    lidarLeft.setTimeout(500);
    lidarLeft.stopContinuous();
    lidarLeft.setScaling(1); // configure range or precision 1, 2 oder 3 mm
    delay(300);
    lidarLeft.startInterleavedContinuous(100);
    delay(100);

    // set up the second LiDAR sensor, on right side
    digitalWrite(LIDAR_RIGHT_SHDN, HIGH);
    delay(50);
    lidarRight.init();
    lidarRight.configureDefault();
    lidarRight.setAddress(0x31); // change to new address
    lidarRight.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    lidarRight.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    lidarRight.setTimeout(500);
    lidarRight.stopContinuous();
    lidarRight.setScaling(1); // configure range or precision 1, 2 oder 3 mm
    delay(300);
    lidarRight.startInterleavedContinuous(100);
    delay(100);

    // set up the third LiDAR sensor, on front side
    digitalWrite(LIDAR_FRONT_SHDN, HIGH);
    delay(50);
    lidarFront.init();
    lidarFront.configureDefault();
    lidarFront.setAddress(0x32); // change to new address
    lidarFront.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    lidarFront.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    lidarFront.setTimeout(500);
    lidarFront.stopContinuous();
    lidarFront.setScaling(1); // configure range or precision 1, 2 oder 3 mm
    delay(300);
    lidarFront.startInterleavedContinuous(100);
    delay(100);
  }

  void tick()
  {
    if (firstTime)
    {
        reset();
        firstTime = false;
    }

    blackboard.lidarFront = lidars[LIDAR_FRONT].readRangeSingleMillimeters();
    blackboard.lidarLeft = lidars[LIDAR_LEFT].readRangeSingleMillimeters();
    blackboard.lidarRight = lidars[LIDAR_RIGHT].readRangeSingleMillimeters();

    // Serial.println("lidars: ");
    // Serial.println(blackboard.lidarFront);
    // Serial.println(blackboard.lidarLeft);
    // Serial.println(blackboard.lidarRight);

    if (!blackboard.startDetected)
    {
        // If we haven't detected the start, keep checking for the start
        bool prevWallInFront = blackboard.wallInFront;
        blackboard.wallInFront = blackboard.lidarFront < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
        blackboard.wallOnLeft = blackboard.lidarLeft < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
        blackboard.wallOnRight = blackboard.lidarRight < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
        if (prevWallInFront == true && blackboard.wallInFront == false)
            blackboard.startDetected = true;
    }
    else if (blackboard.commandCompleted)
    {

        blackboard.lidarFront = blackboard.lidarFront;
        blackboard.lidarLeft = blackboard.lidarLeft;
        blackboard.lidarRight = blackboard.lidarRight;

        // If we've just finished a command, then read lidars
        blackboard.wallInFront = blackboard.lidarFront < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
        blackboard.wallOnLeft = blackboard.lidarLeft < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
        blackboard.wallOnRight = blackboard.lidarRight < MAX_DISTNANCE_TO_CONSIDER_OBSTACLE;
    }
  }

private:
  VL6180X lidars[NUM_LIDARS];
  Blackboard &blackboard;
  bool firstTime;
};

#endif // SENSING_H