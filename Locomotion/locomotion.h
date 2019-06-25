#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include "Arduino.h"
#include <hardware.h>
#include <units.h>

// Define the pins for the left motor and encoder
#define left_motor_en     9
#define left_motor_dirA   6
#define left_motor_dirB   7
#define left_encoder_pinA 2
#define left_encoder_pinB 11

// Define the pins for the right motor and encoder
#define right_motor_en     10
#define right_motor_dirA   4
#define right_motor_dirB   5
#define right_encoder_pinA 3
#define right_encoder_pinB 12

// All the parts
void initEverything();
void part1();
void part2();
void part3();
void part4(int X);
void part5(int Y);

#endif
