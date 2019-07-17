#include "hardware.h"
#include "Arduino.h"

// Set the pin mode
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::config_io_mode(hardware::io_mode mode) -> void {
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    uint8_t mode_int = static_cast<uint8_t>(mode);
    pinMode(pin_number_int, mode_int); // void pinMode(uint8_t pin, uint8_t mode);
}

// Read from digital pin
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::read() -> hardware::logic_level {
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    return digitalRead(pin_number_int) ? // int digitalRead(uint8_t pin);
        hardware::logic_level::low : hardware::logic_level::high; 
}

// Write to digital pin
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::write(hardware::logic_level level) -> void {
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    uint8_t level_int = static_cast<uint8_t>(level);
    digitalWrite(pin_number_int, level_int); // void digitalWrite(uint8_t pin, uint8_t val);
}

// Set pin to high
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::high() -> void {
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    digitalWrite(pin_number_int, 1);
}

// Set pin to low
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::low() -> void {
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    digitalWrite(pin_number_int, 0);
}

// Set the PWM
template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::pwm_write(units::percentage duty_cycle) -> void{
    uint8_t pin_number_int = static_cast<uint8_t>(pin_number);
    int duty_cycle_int = static_cast<int>(duty_cycle.count());
    int val = duty_cycle_int*255/100; // Map the duty cycle ([0, 100]) to an int output ([0, 255])
                                      // Using the floor of this value should be good enough
    analogWrite(pin_number_int, val); // void analogWrite(uint8_t pin, int val);
}

template <hardware::pin_t pin>
auto hardware::digital_pin<pin>::pulse_length (logic_level state = logic_level::high,
    units::microseconds timeout = 1000000_us) -> units::microseconds {
    unsigned long start = millis();
    unsigned long end = millis();
    unsigned long timeout_long = static_cast<unsigned long>(timeout.count());

    while(end - start < timeout_long){ // While there is not a timeout, stay in the loop
        // Check if the pin has changed it's state
        // If it has, return the pulse length in microseconds

        end = millis();
    }

    // Pulse length is > timeout, return timeout length
    return timeout;
}

/**
 * \brief The set_analog_reference method set the reference voltage for
 * analog read.
 * \param ref is the analog reference.
 */
template <typename base>
auto hardware::analog_pin<base>::set_analog_reference (hardware::analog_reference ref) -> void{
    uint8_t ref_int = static_cast<uint8_t>(ref);
    analogReference(ref_int);   //void analogReference(uint8_t mode)
}

/**
 * \brief The analog_read method analog input.
 * \return voltage in volts.
 */
// template<typename base>
// auto hardware::analog_pin<base>::analog_read () -> units::volts{
//     uint8_t pin_number_int = static_cast<uint8_t>(<base> pin_number);
//     double val = analogRead(pin_number_int); // int analogRead(uint8_t pin)
    
//     // For the Arduino Mega, analogRead will return a maximum resolution of 10bits
//     // Scale val to a voltage between within ref_
//     // create a new volts object and return it
// }


/**
 * \brief The enable method initialise the encoder.
 */
template <typename pin_a, typename pin_b>
auto hardware::encoder<pin_a, pin_b>::enable () -> void{
}



// External instantiation for class based on digital_pin
template class hardware::digital_pin<2U>;
template class hardware::digital_pin<13U>;