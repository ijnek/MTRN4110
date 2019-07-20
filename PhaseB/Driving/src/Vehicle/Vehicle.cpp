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
    float leftWheelAngularVelocity;  // left wheel angular velocity (rad/s)
    float rightWheelAngularVelocity;  // right wheel angular velocity (rad/s)
    calculateWheelAngularVelocities(blackboard.movementRequest, leftWheelAngularVelocity, rightWheelAngularVelocity);
    wheelL.setAngularVelocity(leftWheelAngularVelocity);
    wheelR.setAngularVelocity(rightWheelAngularVelocity);

    // Tick the wheels
    wheelL.tick();
    wheelR.tick();

    // Update odometry diff
    blackboard.odometryDiff = calculateOdometryDiff(
        wheelL.getAndResetCounterForOdometry(),
        wheelR.getAndResetCounterForOdometry());
}

/*
 * Calculate left and right wheel angular velocities from movement request
 * This is inverse kinematics
 * https://moodle.telt.unsw.edu.au/pluginfile.php/4264059/mod_resource/content/5/1_MTRN4110_Introduction_Locomotion_Perception.v20190604
 */
void Vehicle::calculateWheelAngularVelocities(
    MovementRequest &movementRequest, float &leftWheelAngularVelocity, float &rightWheelAngularVelocity)
{
    float leftWheelLinearVelocity = movementRequest.forwardVelocity - movementRequest.turnVelocity * DIST_BETWEEN_WHEELS_MM;
    float rightWheelLinearVelocity = movementRequest.forwardVelocity + movementRequest.turnVelocity * DIST_BETWEEN_WHEELS_MM;
    
    leftWheelAngularVelocity = leftWheelLinearVelocity / WHEEL_RADIUS_MM;
    rightWheelAngularVelocity = rightWheelLinearVelocity / WHEEL_RADIUS_MM;
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
    odometryDiff.turn = atan2(rightWheelDistance - leftWheelDistance, DIST_BETWEEN_WHEELS_MM);
    return odometryDiff;
}