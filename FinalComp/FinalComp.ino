#include "src/Utils/MathUtil/MathUtil.h"
#include "src/Types/Blackboard.h"
#include "src/Behaviour/Behaviour.h"
#include "src/Motion/Motion.h"
#include "src/Localisation/Localisation.h"
#include "src/Sensing/Sensing.h"
#include "src/Communication/Communication.h"
#include "src/Exploration/Exploration.h"
#include "src/Planning/Planning.hpp"
#include "src/LED/LED.h"
#include <Wire.h>

Blackboard blackboard;
Sensing sensing(blackboard);
// Receiver receiver(blackboard);
Localisation localisation(blackboard);
Exploration exploration(blackboard);
Planning planning(blackboard);
Behaviour behaviour(blackboard);
Motion motion(blackboard);
LED led(blackboard);

void setup() {
    Serial.begin(9600);  // serial monitor
    Serial1.begin(9600);  // bluetooth communication
    Wire.begin();    
}

void loop() {
    // sensing.tick();
    // receiver.tick();
    // localisation.tick();
    // exploration.tick();
    // planning.tick();
    // behaviour.tick();
    motion.tick();
    // led.tick();

    // String out = "pose: " + String(blackboard.worldPose.x) + ", " + String(blackboard.worldPose.y) + ", " + RAD2DEG(blackboard.worldPose.theta);
    String out = String(blackboard.worldPose.x) + ", " + String(blackboard.worldPose.y) + ", " + RAD2DEG(blackboard.worldPose.theta);
    // Serial1.println(out);

    // Serial1.print("pose: ");
    // Serial1.print(blackboard.worldPose.x);
    // Serial1.print(", ");
    // Serial1.print(blackboard.worldPose.y);
    // Serial.print(", ");
    // Serial.println(RAD2DEG(blackboard.worldPose.theta));
}
