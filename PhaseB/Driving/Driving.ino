#include "src/MathUtil/MathUtil.h"
#include "src/Constants/Constants.h"
#include "src/Blackboard/Blackboard.h"
#include "src/Planning/Planning.h"
#include "src/Vehicle/Vehicle.h"
#include "src/Localisation/Localisation.h"
#include "src/Sensing/Sensing.h"
#include "src/Communication/Communication.h"

Blackboard blackboard;
Communication communication(blackboard);
Sensing sensing(blackboard);
Planning planning(blackboard);
Vehicle vehicle(blackboard);
Localisation localisation(blackboard);

void setup() {
    Serial.begin(9600);  // serial monitor
    Serial1.begin(9600);  // bluetooth communication
}

void loop() {
    // sensing.tick();  // commented out becqause it works better without a gyroscope currently
    communication.tick();
    planning.tick();
    vehicle.tick();
    localisation.tick();

    // Serial.print("pose: ");
    // Serial.print(blackboard.worldPose.x);
    // Serial.print(", ");
    // Serial.print(blackboard.worldPose.y);
    // Serial.print(", ");
    // Serial.println(RAD2DEG(blackboard.worldPose.theta));
}
