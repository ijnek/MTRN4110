#ifndef CONSTANTS_H
#define CONSTANTS_H

// Vehicle dimensions
#define WHEEL_DIAMETER_MM 80.0f                                       // diameter of wheel (mm)
#define WHEEL_RADIUS_MM (WHEEL_DIAMETER_MM / 2)                       // radius of wheel (mm)
#define DIST_BETWEEN_WHEELS_MM 110.0f                                 // distance between centre of two wheels (mm)
#define TYRE_WIDTH_MM 10.0f                                           // width of each tyre (mm)
#define ENCODER_COUNTS_PER_REV 1920                                   // number of counts in single revolution
#define RAD_PER_REV (2 * PI)                                          // number of radians in one revolution
#define DIST_PER_REVOLUTION (WHEEL_DIAMETER_MM * PI)                  // distance a wheel travels in one revolution (mm)
#define DIST_PER_COUNT (DIST_PER_REVOLUTION / ENCODER_COUNTS_PER_REV) // distance a wheel travels in one count (mm)

// PID Controller for motor
#define MOTOR_K_P 0.15
#define MOTOR_K_I 0.12
#define MOTOR_K_D 0.0020

#define PID_SAMPLE_TIME_S 0.1 // duration we sample the velocity of the encoder over (s)

#endif // CONSTANTS_H