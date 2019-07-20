#include "Vehicle.h"
#include "Wheel.h"
#include "../PinDefs/PinDefs.h"
#include "Arduino.h"
#include "../Constants/Constants.h"

// Instances of the wheel, had to do it this way because of the static interrupt functions
Wheel wheelL(ENCODER_PIN_A_L, ENCODER_PIN_B_L, MOTOR_EN_L,
             MOTOR_DIR_A_L, MOTOR_DIR_B_L, true, &leftEncoderInterrupt);
Wheel wheelR(ENCODER_PIN_A_R, ENCODER_PIN_B_R, MOTOR_EN_R,
             MOTOR_DIR_A_R, MOTOR_DIR_B_R, false, &rightEncoderInterrupt);

void Vehicle::tick()
{
    // Set speed of wheels
    float leftWheelSpeed;
    float rightWheelSpeed;
    calculateWheelSpeeds(blackboard.movementRequest, leftWheelSpeed, rightWheelSpeed);
    wheelL.setSpeed(leftWheelSpeed);
    wheelR.setSpeed(rightWheelSpeed);
    wheelL.setDirectionToForwards(true);
    wheelR.setDirectionToForwards(true);

    // Tick the wheels
    wheelL.tick();
    wheelR.tick();

    // Update odometry diff
    blackboard.odometryDiff = calculateOdometryDiff(wheelL.getCounterForOdometry(), wheelR.getCounterForOdometry());
    wheelL.resetCounterForOdometry();
    wheelR.resetCounterForOdometry();
}

/*
 * Calculate left and right wheel speeds from movement request
 * This is inverse kinematics
 * https://moodle.telt.unsw.edu.au/pluginfile.php/4264059/mod_resource/content/5/1_MTRN4110_Introduction_Locomotion_Perception.v20190604
 */
void Vehicle::calculateWheelSpeeds(
    MovementRequest &movementRequest, float &leftWheelSpeed, float &rightWheelSpeed)
{
    leftWheelSpeed = movementRequest.forwardVelocity - movementRequest.turnVelocity * DIST_BETWEEN_WHEELS_MM;
    rightWheelSpeed = movementRequest.forwardVelocity + movementRequest.turnVelocity * DIST_BETWEEN_WHEELS_MM;
}

static void leftEncoderInterrupt()
{
    wheelL.encoderInterrupt();
}

static void rightEncoderInterrupt()
{
    wheelR.encoderInterrupt();
}


Odometry Vehicle::calculateOdometryDiff(int leftCounter, int rightCounter)   // calculate odometry difference since last tick
{
    float leftWheelDistance = leftCounter * DIST_PER_COUNT;
    float rightWheelDistance = rightCounter * DIST_PER_COUNT;

    Odometry odometryDiff;
    odometryDiff.forward = (leftWheelDistance + rightWheelDistance) / 2.0;
    odometryDiff.turn = atan2(rightWheelDistance - leftWheelDistance, (DIST_BETWEEN_WHEELS_MM * 2.0));
    return odometryDiff;
}