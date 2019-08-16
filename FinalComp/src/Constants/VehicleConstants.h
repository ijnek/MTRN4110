#ifndef VEHICLE_CONSTANTS_H
#define VEHICLE_CONSTANTS_H

// Vehicle dimensions
#define WHEEL_DIAMETER_MM 80.0f                                       // diameter of wheel (mm)
#define WHEEL_RADIUS_MM (WHEEL_DIAMETER_MM / 2)                       // radius of wheel (mm)
#define DIST_BETWEEN_WHEELS_MM 110.0f                                 // distance between centre of two wheels (mm)
#define TYRE_WIDTH_MM 10.0f                                           // width of each tyre (mm)
#define ENCODER_COUNTS_PER_REV 1920                                   // number of counts in single revolution
#define RAD_PER_REV (2 * PI)                                          // number of radians in one revolution
#define DIST_PER_REVOLUTION (WHEEL_DIAMETER_MM * PI)                  // distance a wheel travels in one revolution (mm)
#define DIST_PER_COUNT (DIST_PER_REVOLUTION / ENCODER_COUNTS_PER_REV) // distance a wheel travels in one count (mm)
#define WHEELS_OFFSET_X_MM 17                                         // how much robot's wheels are offset forwards, relative to centre of vehicle (mm)

// PID Controller for motor
#define PID_SAMPLING_PERIOD_MS 50 // duration we sample the velocity of the encoder over (ms)
#define MOTOR_K_P 4.50
#define MOTOR_K_I 0.00
#define MOTOR_K_D 8.00
#define MAX_MOTOR_PWM_OUTPUT 130 // Maximum output pwm value of a motor, increase to go faster, lower to go slower
// #define MAX_MOTOR_PWM_OUTPUT 80 // Maximum output pwm value of a motor, increase to go faster, lower to go slower

// Some multipliers to make odometry estimate position better
#define ODOMETRY_FORWARD_MULTIPLIER 1.05
#define ODOMETRY_TURN_MULTIPLIER 0.98

// Some multipliers to make left and right motor spin more evenly
#define LEFT_MOTOR_PWM_MULTIPLIER 1.00
#define RIGHT_MOTOR_PWM_MULTIPLIER 1.08

#endif // VEHICLE_CONSTANTS_H