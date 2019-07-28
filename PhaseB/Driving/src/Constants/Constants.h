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
#define WHEELS_OFFSET_X_MM 17                                         // how much robot's wheels are offset forwards, relative to centre of vehicle (mm)

// Maze dimensions
#define CELL_SIZE 250 // size of cell (mm)

// PID Controller for motor
#define PID_SAMPLING_PERIOD_MS 50 // duration we sample the velocity of the encoder over (ms)
#define MOTOR_K_P 4.00
#define MOTOR_K_I 0.00
#define MOTOR_K_D 8.00
#define MAX_MOTOR_PWM_OUTPUT 80 // Maximum output pwm value of a motor, increase to go faster, lower to go slower

// Some multipliers to make odometry estimate position better
#define ODOMETRY_FORWARD_MULTIPLIER 1.00
#define ODOMETRY_TURN_MULTIPLIER 0.98

// Some multipliers to make left and right motor spin more evenly
#define LEFT_MOTOR_PWM_MULTIPLIER 1.08
#define RIGHT_MOTOR_PWM_MULTIPLIER 1.00

// Bluetooth communication / Plans
#define PLAN_NONE 0          // don't do anything
#define PLAN1 1              // move forwards 1 block
#define PLAN2 2              // turn, according to command given in bluetooth
#define PLAN3 3              // move forwards 6 blocks
#define PLAN4 4              // do the snakey course
#define PLAN_TURN_LEFT 5     // turn left 90 degrees
#define PLAN_TURN_RIGHT 6    // turn right 90 degrees
#define PLAN_FORWARDS 7      // go forwards
#define PLAN_NEXT_WAYPOINT 8 // plan using the "nextWayPoint" variable on the blackboard

#endif // CONSTANTS_H