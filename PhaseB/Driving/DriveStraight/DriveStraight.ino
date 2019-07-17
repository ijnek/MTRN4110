#include "PinDefs.hpp"
#include "Motor.hpp"

Motor motorL(ENCODER_PIN_A_L, ENCODER_PIN_B_L, MOTOR_EN_L, MOTOR_DIR_A_L, MOTOR_DIR_B_L, true);
Motor motorR(ENCODER_PIN_A_R, ENCODER_PIN_B_R, MOTOR_EN_R, MOTOR_DIR_A_R, MOTOR_DIR_B_R, false);


void setup() {
}

void loop() {

    motorL.setDirectionToForwards(true);
    motorL.setSpeedByUint8_t(200);

    motorR.setDirectionToForwards(true);
    motorR.setSpeedByPercentage(80);

    delay(1000);
    
}
