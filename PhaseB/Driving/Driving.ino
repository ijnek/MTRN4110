#include "src/Blackboard/Blackboard.h"
#include "src/Planning/Planning.h"
#include "src/Vehicle/Vehicle.h"
#include "src/Localisation/Localisation.h"

Blackboard blackboard;
Planning planning(blackboard);
Vehicle vehicle(blackboard);
Localisation localisation(blackboard);

void setup() {
    Serial.begin(9600);
}

void loop() {
    planning.tick();
    vehicle.tick();
    localisation.tick();
    delay(50);  // remove this later
}
