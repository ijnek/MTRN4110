#include "src/Vehicle/Vehicle.h"

Vehicle vehicle;

void setup() {
    Serial.begin(9600);

}

void loop() {
    vehicle.tick();
    delay(10);
}
