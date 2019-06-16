#include "hardware.h"
#include "Arduino.h"

// Set the pin mode
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::config_io_mode(hardware::io_mode mode) -> void {
    // void pinMode(uint8_t pin, uint8_t mode);
    pinMode(pin_number, uint8_t(mode));
}

// Read from digital pin
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::read() -> hardware::logic_level {
    // int digitalRead(uint8_t pin);
    return digitalRead(uint8_t(pin_number)) ? hardware::logic_level::low : hardware::logic_level::high;
}

// Write to digital pin
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::write(hardware::logic_level level) -> void {
    // void digitalWrite(uint8_t pin, uint8_t val);
    digitalWrite(pin_number, uint8_t(level));
}

// Set pin to high
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::high() -> void {
    digitalWrite(pin_number, hardware::logic_level::high);
}

// Set pin to low
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::low() -> void {
    digitalWrite(pin_number, hardware::logic_level::low);
}

// External instantiation for class templates
template class hardware::digital_pin<2U>;