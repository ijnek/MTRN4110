#include "src/MathUtil/MathUtil.h"
#include "src/Constants/Constants.h"
#include "src/Blackboard/Blackboard.h"
#include "src/Planning/Planning.h"
#include "src/Vehicle/Vehicle.h"
#include "src/Localisation/Localisation.h"
#include "src/Sensing/Sensing.h"
#include "src/Communication/Communication.h"
#include "src/Exploration/Exploration.h"
#include "src/LED/LED.h"
#include <Wire.h>

Blackboard blackboard;
Communication communication(blackboard);
Sensing sensing(blackboard);
Exploration exploration(blackboard);
Planning planning(blackboard);
Vehicle vehicle(blackboard);
Localisation localisation(blackboard);
LED led(blackboard);

void setup() {
    Serial.begin(9600);  // serial monitor
    Serial1.begin(9600);  // bluetooth communication
    Wire.begin();    
}

void loop() {
    communication.tick();
    sensing.tick();
    exploration.tick();
    planning.tick();
    vehicle.tick();
    localisation.tick();
    led.tick();

    // Serial.print("pose: ");
    // Serial.print(blackboard.worldPose.x);
    // Serial.print(", ");
    // Serial.print(blackboard.worldPose.y);
    // Serial.print(", ");
    // Serial.println(RAD2DEG(blackboard.worldPose.theta));
}
