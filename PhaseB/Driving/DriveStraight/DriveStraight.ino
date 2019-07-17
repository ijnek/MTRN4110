#include "PinDefs.hpp"
#include "Wheel.hpp"

Wheel wheelL(ENCODER_PIN_A_L, ENCODER_PIN_B_L, MOTOR_EN_L, MOTOR_DIR_A_L, MOTOR_DIR_B_L, true);
Wheel wheelR(ENCODER_PIN_A_R, ENCODER_PIN_B_R, MOTOR_EN_R, MOTOR_DIR_A_R, MOTOR_DIR_B_R, false);


void setup() {
}

void loop() {

    wheelL.setDirectionToForwards(true);
    wheelL.setSpeedByPercentage(20);

    wheelR.setDirectionToForwards(true);
    wheelR.setSpeedByPercentage(20);


    delay(1000);
}
