#pragma once
// __STUDENT_SHOULD_CHANGE_THIS_FILE__
// Example of how hardware definition could look like.

namespace hardware
{
/**
 * \brief The pins namespace contains definition of the functionality of all the
 * pins on Arduino.
 */
namespace pins    // TODO student to define.
{
/**
 * \brief The imu_interrupt is used by imu to signal data ready.
 */
using imu_interrupt = digital_pin<1U>;

/**
 * \brief The in1 is motor drive channel 1 direction
 */
using in1 = digital_pin<2U>;

/**
 * \brief The in2 is motor drive channel 1 speed
 */
using in2 = digital_pin<3U>;

/**
 * \brief The in3 is motor drive channel 2 direction
 */
using in3 = digital_pin<4U>;

/**
 * \brief The in4 is motor drive channel 2 speed
 */
using in4 = digital_pin<5U>;

/**
 * \brief The sonar_trigger is ultrasound trigger pin connected to a0.
 */
using sonar_trigger = analog_pin<digital_pin<6U>>;

/**
 * \brief The ultrasound_echo is ultrasound echo pin connected to a1.
 */
using sonar_echo = analog_pin<digital_pin<7U>>;

using left_encoder_a = interrupt<digital_pin<8U>>;
using left_encoder_b = digital_pin<9U>;

using right_encoder_a = interrupt<digital_pin<10U>>;
using right_encoder_b = digital_pin<11U>;

using left_lidar_enable = digital_pin<12U>;
using right_lidar_enable = digital_pin<13U>;

constexpr auto left_lidar_address = 0x22;
constexpr auto right_lidar_address = 0x42;
}    // namespace pins

/**
 * \brief The led is on board led pin
 */
using led = digital_pin<13U>;

using left_encoder = encoder<pins::left_encoder_a, pins::left_encoder_b>;
using left_wheel = wheel<pins::left_encoder_a, pins::left_encoder_b>;

using right_encoder = encoder<pins::right_encoder_a, pins::right_encoder_b>;
using right_wheel = wheel<pins::right_encoder_a, pins::right_encoder_b>;

/**
 * \brief The left_motor bind left motor to the correct pins.
 */
using right_motor = motor<pins::in1, pins::in2>;

/**
 * \brief The right_motor bind right motor to the correct pins.
 */
using left_motor = motor<pins::in3, pins::in4>;

/**
 * \brief The ultrasound is ultrasound sensor template bind to the trigger and
 * echo pin.
 */
using front_sonar = sonar<pins::sonar_trigger, pins::sonar_echo>;

/**
 * \brief This serial is used to communicate with pc via usb.
 */
using serial = serial_api<serial_tag<0>>;

/**
 * \brief The serial is used to communicate with Bluetooth module.
 */
using bluetooth = serial_api<serial_tag<1>>;

using left_lidar = lidar<lidar_tag<0>>;

using right_lidar = lidar<lidar_tag<1>>;
}    // namespace hardware
