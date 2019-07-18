#include "src/Vehicle/Vehicle.h"

Vehicle vehicle;

void setup() {
    Serial.begin(9600);

    // Add some commands to queue
    vehicle.addCommand(DriveCommand::FORWARD);
    vehicle.addCommand(DriveCommand::TURN_LEFT);
    vehicle.addCommand(DriveCommand::BACKWARD);
    vehicle.addCommand(DriveCommand::TURN_RIGHT);
}

void loop() {
    vehicle.tick();
}
