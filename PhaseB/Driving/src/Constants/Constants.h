#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WHEEL_DIAMETER_MM 80.0f  // diameter of wheel (mm)
#define DIST_BETWEEN_WHEELS_MM 110.0f // distance between centre of two wheels (mm)
#define TYRE_WIDTH_MM 10.0f  // width of each tyre (mm)
#define ENCODER_COUNTS_PER_REV 1920  // number of counts in single revolution
#define DIST_PER_REVOLUTION WHEEL_DIAMETER_MM*3.1415  // distance a wheel travels in one revolution (mm)
#define DIST_PER_COUNT DIST_PER_REVOLUTION/ENCODER_COUNTS_PER_REV  // distance a wheel travels in one count (mm)

// PID Controller for motor
#define MOTOR_K_P 0.6
#define MOTOR_K_I 5.0
#define MOTOR_K_D 0.0

#endif // CONSTANTS_H