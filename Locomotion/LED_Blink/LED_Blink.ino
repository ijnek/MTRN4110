#include <hardware.h>

void setup() {
  // Configure digital pin 2 as an output
  hardware::io_mode mode = hardware::io_mode::output;
  hardware::pins::in1::config_io_mode(mode);
}

void loop() {
  // Turn LED on and off
  hardware::pins::in1::write(hardware::logic_level::high); // Turn LED on
  delay(2000); // Wait for 2 seconds
  hardware::pins::in1::write(hardware::logic_level::low);
  delay(2000);
}
