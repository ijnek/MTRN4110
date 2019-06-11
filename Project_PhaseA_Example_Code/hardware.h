#pragma once
// __DO_NOT_CHANGE_THIS_FILE__
#define MTRN_HARDWARE_VERSION 1.0

#include "type_traits.h"
#include "units.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <stdint.h>    // NOLINT(modernize-deprecated-headers, hicpp-deprecated-headers)
// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>    // NOLINT(modernize-deprecated-headers, hicpp-deprecated-headers)
using namespace units::literals;

/**
 * \brief The hardware namespace is the low level api used to control all the
 * hardware.
 */
namespace hardware
{
/**
 * \brief io_mode is represents digital I/O modes.
 */
enum class io_mode : uint8_t
{
    /** input mode with internal pull up resistor disabled*/
    input = 0x00U,
    /** output mode*/
    output = 0x01U,
    /** input mode with internal pull up resistor enabled.*/
    input_pullup = 0x02U,
    /** I/O mode not set yet.*/
    unset = 0xFF
};

/**
 * logic_level
 * \brief logic_level represents digital logic level.
 */
enum class logic_level : uint8_t
{
    /*! Input mode: < 1.5v for 5V board, < 1.0v for 3.3v board.
     * Output mode: GND*/
    low = 0,
    /*! Input mode: > 3.0v for 5V board, > 2.0v for 3.3v board.
     * Output mode: VCC*/
    high = 1
};

/**
 * \brief pin_t represents Arduino pin number.
 */
using pin_t = uint8_t;
}    // namespace hardware

namespace hardware
{
/**
 * \brief The digital_pin class template represents ownership of digital I/O
 * pins.
 * \tparam pin is the digital I/O pin owned by the class instantiation.
 */
template <pin_t pin>
class digital_pin
{
    public:
    /**
     * \brief pin_number assigned to the instantiation of digital_pin
     */
    static constexpr auto pin_number = pin;

    /**
     * \brief The config_io_mode method set I/O mode to the digital pin.
     * Should be called to set the correct mode before any I/O activity.
     * \param mode is the desired I/O mode.
     */
    static auto config_io_mode (io_mode mode) -> void;

    /**
     * \brief The read method returns the current pin input.
     * \return the input logic level.
     */
    static auto read () -> logic_level;

    /**
     * \brief The write method set digital pin output to mode.
     * \param level is the logic level to set the digital pin to.
     */
    static auto write (logic_level level) -> void;

    /**
     * \brief The high method set digital output level to high.
     */
    static auto high () -> void;

    /**
     * \brief The low method set digital output level to low.
     */
    static auto low () -> void;

    /**
     * \brief The pwm_write method writes an analog voltage out as PWM wave.
     * This function does not check if the pin is capable of PWM.
     * Uno pin 3, 5, 6, 9, 10, 11. Mega: 2 - 13 and 44 - 46.
     * PWM at 490Hz except pin 5 and 6 are 980Hz.
     * \param duty_cycle value between 0.0 and 100.0 representing is the PWM
     * duty cycle of 0% to 100%.
     */
    static auto pwm_write (units::percentage duty_cycle) -> void;

    /**
     * \brief pulse_length measures the duration of a pulse in microseconds.
     * With either rising or falling edge as trigger.
     * \param state HIGH for rising edge trigger, LOW for falling edge trigger
     * start.
     * \param timeout in microseconds
     * \return pulse length in
     * microseconds.
     */
    static auto pulse_length (logic_level state = logic_level::high,
        units::microseconds timeout = 1000000_us) -> units::microseconds;
};

/**
 * \brief The analog_reference type represents all possible analog read
 * reference voltage. Some voltages are not available on certain boards.
 */
enum class analog_reference : uint8_t
{
    /*! VCC*/
    vcc_default,
    /*! 1.1v on ATmega328P, not available on Mega.*/
    internal,
    /*! 1.1v on Mega only.*/
    internal_1v1,
    /*! 2.56v on Mega only.*/
    internal_2v56,
    /*! AREF.*/
    external
};

/**
 * \brief The analog_pin class template extends digital_pin class
 * template to add analog read functionality.
 * \tparam base should be an instantiation of digital_pin class.
 */
template <typename base>
class analog_pin : public digital_pin<base::pin_number>
{
    static constexpr auto conversion_unit = 1024;
    static analog_reference ref_;

    public:
    /**
     * \brief The set_analog_reference method set the reference voltage for
     * analog read.
     * \param ref is the analog reference.
     */
    static auto set_analog_reference (analog_reference ref) -> void;

    /**
     * \brief The analog_read method analog input.
     * \return voltage in volts.
     */
    static auto analog_read () -> units::volts;
};

/**
 * \brief interrupt_mode represents the type of interrupt trigger.
 */
enum class interrupt_mode : int
{
    /** Trigger when the input is low*/
    low = 0,
    /** Trigger when the pin changes value*/
    change = 1,
    /** Trigger when the input goes from low to high*/
    rising = 2,
    /** Trigger when the input goes from high to low*/
    falling = 3
};

/**
 * \brief The interrupt class template extends digital_pin to handle interrupt
 * \tparam pin should be an instantiation of digital_pin class.
 */
template <typename pin>
class interrupt : public digital_pin<pin::pin_number>
{
    public:
    /**
     * \brief The attach_interrupt method set the interrupt service routine
     * (ISR) that will be called when an interrupt is triggered.
     * \param callback
     * function with function signature of void meow ();
     * \param mode is the
     * interrupt trigger condition.
     */
    static auto attach_interrupt (void (*callback) (),
        interrupt_mode mode = interrupt_mode::rising) -> void;

    /**
     * \brief The detach_interrupt method remove the interrupt service routine
     * (ISR) associated with the pin.
     */
    static auto detach_interrupt () -> void;
};

/**
 * \brief The motor represents one motor channel of the motor driver. See motor
 * driver data sheets for digital output combination to control the direction of
 * motor. \tparam pin_a is direction pin a \tparam pin_b is direction pin b
 */
template <class pin_a, class pin_b>
class motor
{
    public:
    /**
     * \brief The enable method enables motor control pins.
     */
    static auto enable () -> void;

    /**
     * \brief The stop method stops the motor
     */
    static auto stop () -> void;

    /**
     * \brief The forward method makes the motor goes forward.
     * \param velocity is percentage of maximum speed of motor.
     */
    static auto forward (units::percentage velocity) -> void;

    /**
     * \brief The forward method makes the motor goes backward.
     * \param velocity is percentage of maximum speed of motor.
     */
    static auto backward (units::percentage velocity) -> void;
};

/**
 * \brief The drive_direction enum defines the direction of travel.
 */
enum class drive_direction : uint8_t
{
    unknown,
    forward,
    backward
};

/**
 * \brief The encoder_count is a type representing the count of encoder
 */
using encoder_count = int;

/**
 * \brief The encoder class template reads the encoder value
 * \tparam pin_a is the pin connected to encoder sensor A, should be a
 * digital_pin \tparam pin_b is the pin connected to encoder sensor B, should be
 * a digital_pin
 */
template <typename pin_a, typename pin_b>
class encoder
{
    using encoder_pin_a = interrupt<digital_pin<pin_a::pin_number>>;
    using encoder_pin_b = digital_pin<pin_b::pin_number>;

    public:
    /**
     * \brief The enable method initialise the encoder.
     */
    static auto enable () -> void;

    /**
     * \brief The count method returns the current count of the encoder.
     * \return current count.
     */
    static auto count () -> encoder_count;

    /**
     * \brief The reset method reset the current count and position of the
     * encoder.
     */
    static auto reset () -> void;
};

/**
 * \brief The wheel class template extends encoder to include the wheel
 * parameters \tparam pin_a is the pin connected to encoder sensor A, should be
 * a digital_pin \tparam pin_b is the pin connected to encoder sensor B, should
 * be a digital_pin
 */
template <typename pin_a, typename pin_b>
class wheel : public encoder<pin_a, pin_b>
{
    public:
    /**
     * \brief The position method returns the distance traveled by the wheel,
     * assume no slip. \return the distance traveled by the wheel
     */
    static auto position () -> units::millimeters;
};
}    // namespace hardware

namespace hardware
{
/**
 * \brief The i2c class is used to config the Arduino Wire i2c communication
 * library.
 */
class i2c
{
    public:
    /**
     * \brief The address_t type represents the address of an i2c device.
     */
    using address_t = uint8_t;

    /**
     * \brief The clock_t type represents the i2c clock
     */
    using clock_t = uint32_t;

    /**
     * \brief The fast_mode defined the clock rate for i2c fast mode.
     */
    static constexpr clock_t fast_mode = 400000;

    /**
     * \brief The begin method initialise i2c bus.
     */
    static auto enable () -> void;

    /**
     * \brief The clock method sets the i2c bus communication rate.
     * \param clock is the new i2c clock rate.
     */
    static auto clock (clock_t clock) -> void;
};

/**
 * \brief The imu class is used to config and use the inertial measurement unit.
 */
class imu
{
    public:
    /**
     * \brief The enable method initialise the imu.
     * \return true if imu is successfully initialized.
     */
    static auto enable () -> bool;

    /**
     * \brief The update method updates the roll, pitch, yaw angle to the latest
     * value from IMU.
     * \return true if there were new values.
     */
    static auto update () -> bool;

    /**
     * \brief The yaw method gets the current yaw value
     * \return the yaw value from last update.
     */
    static auto yaw () -> float;

    /**
     * \brief The yaw method gets the current yaw value
     * \return the yaw value from last update.
     */
    static auto yaw (float value) -> void;

    /**
     * \brief The pitch method gets the current pitch value
     * \return the pitch value from last update.
     */
    static auto pitch () -> float;

    /**
     * \brief The roll method gets the current roll value
     * \return the roll value from last update.
     */
    static auto roll () -> float;

    /**
     * \brief The stabilize method waits until the imu is stabilized.
     */
    static auto stabilize () -> void;
};

/**
 * \brief The lidar_tag is used to instantiate range_sensor template to
 * each individual range sensor.
 * \tparam id is the range sensor number.
 */
template <uint8_t id>
class lidar_tag;

/**
 *
 * \brief The lidar class read the distance from an range sensor
 * \tparam tag is used to instantiate the template for each physical sensor. Tag
 * need to be range_sensor_tag<value here>
 */
template <typename tag>
class lidar
{
    // Ignore this, validate the type of tag is correct.
    static_assert (check_tag<tag, uint8_t, lidar_tag>::value,
        "range_sensor must be templated by range_sensor_tag.");

    public:
    /**
     * \brief The enable function initialise the range sensor.
     */
    static auto enable () -> void;

    /**
     * \brief The distance function return the measured distance.
     * \return the distance of detected object in front of thee sensor in
     * millimeters
     */
    static auto distance () -> units::millimeters;
};

/**
 * \brief The sonar class interfaces with the ultrasound module. See
 * data sheet on how ultrasound range finder works.
 * \tparam trigger_pin is a analog_pin connected to the trigger pin of the
 * ultrasound module. \tparam echo_pin is a analog_pin connected to the
 * echo pin of the ultrasound module
 */
template <class trigger_pin, class echo_pin>
class sonar
{
    public:
    /**
     * \brief The enable the ultrasound.
     */
    static auto enable () -> void;

    /**
     * \brief The distance trigger the ultrasound module to get one distance
     * measurement. \return measured distance in cm.
     */
    static auto distance () -> units::millimeters;
};
}    // namespace hardware

namespace hardware    // UART section
{
/**
 * \brief The serial_tag class instantiate serial_api to the specific serial
 * port.
 * \tparam id is the serial number.
 */
template <uint8_t id>
class serial_tag;

/**
 * \brief The char_count type represents the number of bytes written or
 * printed by serial.
 */
using char_count = unsigned int;

/**
 * \brief The serial_api template class defines the common API for all serial
 * like streams. Delegate to the implementation to do the real work.
 * \tparam tag is the type of actual implementation of serial. tag need to be
 * serial_tag<value here>
 */
template <typename tag>
class serial_api
{
    // Ignore this, validate the type of tag is correct.
    static_assert (check_tag<tag, uint8_t, serial_tag>::value,
        "serial_api must be templted by serial_tag.");

    public:
    /**
     * \brief Print a string to serial as is.
     * \param string is a null terminated string.
     * \return total number of bytes written.
     */
    static auto print (char const * string) -> char_count;

    /**
     * \brief Print a character to serial as is.
     * \param c is the character to print.
     * \return total number of bytes written.
     */
    static auto print (char c) -> char_count;

    /**
     * \brief Print a signed int to serial as human readable text.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print (int i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned int to serial as human readable text.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print (unsigned int i, int base = 10) -> char_count;

    /**
     * \brief Print a signed long int to serial as human readable text.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print (long i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned long int to serial as human readable text.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print (unsigned long i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned char to serial as human readable text.
     * \param c is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print (unsigned char c, int base = 10) -> char_count;

    /**
     * \brief Print a double to serial as human readable text.
     * \param i is the value to be printed.
     * \param base number of decimal place to print to.
     * \return total number of bytes written.
     */
    static auto print (double i, int base = 2) -> char_count;

    /**
     * \brief Print a string to serial followed by return character.
     * \param string is a null terminated string.
     * \return total number of bytes written.
     */
    static auto print_line (char const * string) -> char_count;

    /**
     * \brief Print a character to serial as is followed by return character.
     * \param c is the character to print.
     * \return total number of bytes written.
     */
    static auto print_line (char c) -> char_count;

    /**
     * \brief Print a signed int to serial as human readable text followed by
     * return character.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print_line (int i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned int to serial as human readable text followed by
     * return character.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print_line (unsigned int i, int base = 10) -> char_count;

    /**
     * \brief Print a signed long int to serial as human readable text followed
     * by return character.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print_line (long i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned long int to serial as human readable
     * text followed by return character.
     * \param i is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print_line (unsigned long i, int base = 10) -> char_count;

    /**
     * \brief Print an unsigned char to serial as human readable text followed
     * by return character.
     * \param c is the value to be printed.
     * \param base is the number base to use. Can be 2,6,8,10.
     * \return total number of bytes written.
     */
    static auto print_line (unsigned char c, int base = 10) -> char_count;

    /**
     * \brief Print a double to serial as human readable text followed by return
     * character.
     * \param i is the value to be printed.
     * \param base number of decimal place to print to.
     * \return total number of bytes written.
     */
    static auto print_line (double i, int base = 2) -> char_count;

    /**
     * \brief write a unsigned long int to serial as a series of bytes.
     * \param n is the value.
     * \return total number of bytes written.
     */
    static auto write (unsigned long n) -> char_count;

    /**
     * \brief write a signed long int to serial as a series of bytes.
     * \param n is the value.
     * \return total number of bytes written.
     */
    static auto write (long n) -> char_count;

    /**
     * \brief write a int to serial as a series of bytes.
     * \param n is the value.
     * \return total number of bytes written.
     */
    static auto write (int n) -> char_count;

    /**
     * \brief write a unsigned byte to serial.
     * \param n is the value.
     * \return total number of bytes written.
     */
    static auto write (uint8_t n) -> char_count;

    /**
     * \brief write a string to serial as a series of byte.
     * \param str is a null terminated string.
     * \return total number of bytes written.
     */
    static auto write (const char * str) -> char_count;

    /**
     * \brief write n bytes to serial.
     * \param string is an array of char.
     * \param size is number of bytes to send.
     * \return total number of bytes written.
     */
    static auto write (char const * string, size_t size) -> char_count;

    /**
     * \brief The begin method set the data rate of hardware serial connection
     * and start communication.
     * \param baud in bits per seconds. Common values include 9600,38400,115200,
     */
    static auto enable (unsigned long baud = 115200) -> void;

    /**
     * \brief The end method stop serial communication.
     */
    static auto end () -> void;

    /**
     * \brief The available method returns number of bytes available for read.
     * \return number of bytes available to read.
     */
    static auto input_byte_in_buffer () -> int;

    /**
     * \brief The available_for_write method returns the number of bytes
     * available for write without blocking operation. Essentially the amount of
     * space left in the buffer.
     * \return number of bytes.
     */
    static auto output_buffer_space () -> int;

    /**
     * \brief The peek method returns the next byte in the stream without
     * removing it from the internal buffer.
     * \return the next byte
     */
    static auto peek () -> int;

    /**
     * \brief The read method return the next character from stream.
     * \return the next byte
     */
    static auto read () -> int;

    /**
     * \brief The read_bytes method read from stream until either length char
     * have been read, or times out;
     * \param buffer is a pointer to the return buffer.
     * \param length is the maximum number of bytes to read.
     * \return number of character read.
     */
    static auto read_bytes (char * buffer, char_count length) -> char_count;

    /**
     * \brief The read_bytes_until method read from stream until either
     * terminator has been read, timeout or length character have been read.
     * \param terminator is the terminator to look for.
     * \param buffer is a pointer to the return buffer.
     * \param length is the maximum number of bytes to read.
     * \return number of bytes read, not including terminator.
     */
    static auto read_bytes_until (
        char terminator, char * buffer, char_count length) -> char_count;

    /**
     * \brief The timeout_duration method sets the maximum milliseconds to wait
     * for serial data.
     * \param timeout in milliseconds.
     */
    static auto timeout_duration (units::milliseconds timeout) -> void;

    /**
     * \brief The flush method wait until transmission of outgoing serial data
     * is complete.
     */
    static auto flush () -> void;

    /**
     * \brief The clear method clear the command line console. Do nothing in
     * the Arduino version.
     */
    static auto clear () -> void;
};

/**
 * \brief The maze_layout_message class holds the message received from
 * Bluetooth about the maze. Student to design the class.
 */
class maze_layout_message;

/**
 * \brief The maze_layout class holds the parsed layout of the maze.
 */
class maze_layout;

/**
 * \brief The receive_maze_layout function is called to receive the layout of
 * the maze via Bluetooth. \return the message encoding the maze layout.
 */
auto receive_maze_layout () -> maze_layout_message;

/**
 * \brief cell_location class describe the position of a cell.
 */
class cell_location;

/**
 * \brief The parse_maze_layout function parses the message received from
 * Bluetooth into maze_layout. \return the parsed layout of the maze.
 */
auto parse_maze_layout (maze_layout_message maze) -> maze_layout;

/**
 * \brief The display class is used to display messages on an I2C LCD display.
 */
class display
{
    public:
    /**
     * \brief Row and column position on the lcd display.
     */
    struct coordinate
    {
        int row;
        int column;
    };

    /**
     * \brief Initialise the lcd display.
     */
    static auto enable () -> void;

    /**
     * \brief
     * \param cursor_position method set the cursor position.
     */
    static auto cursor (coordinate cursor_position) -> void;

    /**
     * \brief print a string
     * \param string c string to be displayed.
     * \return number of char printed.
     */
    static auto print (char const * string) -> size_t;

    /**
     * \brief print number in in decimal format.
     * \param n is the number to be printed.
     * \return number of char printed.
     */
    static auto print (int n) -> size_t;

    /**
     * \brief print double in in decimal format.
     * \param n is the number to be printed.
     * \return number of char printed.
     */
    static auto print (double n) -> size_t;

    /**
     * \brief print a message about the layout of a cell (ie is the cell open or
     * closed in the four directions). \param maze is the layout of the maze
     * \param cell is the cell to be printed.
     * \return number of char printed.
     */
    static auto print (maze_layout maze, cell_location cell) -> size_t;

    /**
     * \brief clear the lcd display.
     */
    static auto clear () -> void;
};
}    // namespace hardware

// ReSharper disable once CppUnusedIncludeDirective
#include "hardware_definition.h"
