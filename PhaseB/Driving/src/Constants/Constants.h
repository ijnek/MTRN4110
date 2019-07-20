#ifndef CONSTANTS_H
#define CONSTANTS_H

// Some conversions
#define MS_TO_S(ms) (ms * 1000.0)
#define S_TO_MS(s) (s / 1000.0)
#define CLAMP(min_val, val, max_val) min(max(min_val, val), max_val)
#define RAD2DEG(val) (val*180.0/PI)
#define DEG2RAD(val) (val*PI/180.0)

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
#define MOTOR_K_P 0.01
#define MOTOR_K_I 0.10
#define MOTOR_K_D 0.0
#define PID_SAMPLE_TIME_S 0.1 // duration we sample the velocity of the encoder over (s)

#endif // CONSTANTS_H