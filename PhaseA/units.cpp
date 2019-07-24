#include "units.h"
#define PI  3.1415

/**
 * \brief The operator+ adds two units objects of the same type together.
 * \param rhs is the unit_type to add.
 * \return a new object of the same unit_type storing the result.
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator+(unit_type rhs) const -> unit_type {
    unit_type unit(value_ + rhs.count());
    return unit;
}

/**
 * \brief The operator- subtracts two units objects of the same type together.
 * \param rhs is the unit_type to subtract.
 * \return a new object of the same unit_type storing the result.
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator-(unit_type rhs) const -> unit_type {
    unit_type unit(value_ - rhs.count());
    return unit;
}

/**
 * \brief The operator* multiplies two units objects of the same type together.
 * \param rhs is the value_type to multiply.
 * \return a new object of the same unit_type storing the result.
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator*(value_type rhs) const -> unit_type {
    unit_type unit(value_ * rhs);
    return unit;
}

/**
 * \brief The operator/ divides two units objects of the same type together.
 * \param rhs is the value_type to divide.
 * \return a new object of the same unit_type storing the result.
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator/(value_type rhs) const -> unit_type {
    unit_type unit(value_ / rhs);
    return unit;
}

/**
 * \brief The operator< compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs < rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator<(unit_type rhs) const -> bool {
    return value_<rhs.count() ? true:false;
}

/**
 * \brief The operator<= compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs <= rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator<=(unit_type rhs) const -> bool {
    return value_<=rhs.count() ? true:false;
}

/**
 * \brief The operator> compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs > rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator>(unit_type rhs) const -> bool {
    return value_>rhs.count() ? true:false;
}

/**
 * \brief The operator>= compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs >= rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator>=(unit_type rhs) const -> bool {
    return value_>=rhs.count() ? true:false;
}

/**
 * \brief The operator== compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs == rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator==(unit_type rhs) const -> bool {
    return value_==rhs.count() ? true:false;
}

/**
 * \brief The operator!= compares two units of the same type.
 * \param rhs is the right hand side value
 * \return true if lhs != rhs
 */
template <typename derived_units_name, typename value_t>
auto units::base_unit<derived_units_name, value_t>::operator!=(unit_type rhs) const -> bool {
    return value_!=rhs.count() ? true:false;
}
/**
 * \brief The now method returns the number of microseconds since the
 * program started. Overflow approximately every 70 minutes
 * \return microseconds since start of program
 */
auto units::microseconds::now() noexcept -> microseconds {
    using namespace units::literals;
    return 1_us;
}

/**
 * \brief The now  method returns the number of microseconds since the
 * program started. Overflow approximately every 50 days.
 * \return milliseconds since start of program
 */
auto units::milliseconds::now() noexcept -> milliseconds {
    using namespace units::literals;
    return 1_ms;
}

/**
 * \brief The sleep_for method pause the program for period of time.
 * \param period is the number of milliseconds to pause the program.
 */
auto units::sleep_for(units::milliseconds period) -> void {
}

/**
 * \brief The sleep_for method pause the program for period of time.
 * \param period is the number of microseconds to pause the program.
 */
auto units::sleep_for(units::microseconds period) -> void {
}

/**
 * \brief The sleep_until pause the program until a specific time since start of
 * program
 * \param wake_time is the time to resume execution in milliseconds.
 */
auto units::sleep_until(units::milliseconds wake_time) -> void {
}

/**
 * \brief The sleep_until pause the program until a specific time since start of
 * program
 * \param wake_time is the time to resume execution in microseconds.
 */
auto units::sleep_until(units::microseconds wake_time) -> void {
}


/**
 * \brief The operator degrees () converts a radians object into a degrees object.
 * \return a new degrees object.
 */
units::radians::operator degrees () const {
    degrees deg(count() * 180/PI);
    return deg;
}

/**
 * \brief The operator radians () converts a degrees object into a radians object.
 * \return a new radians object.
 */
units::degrees::operator radians () const {
    radians rad(count() * PI/180);
    return rad;
}

// External instantiation for classes based on base_unit
template class units::base_unit<units::microseconds, unsigned long>;
template class units::base_unit<units::milliseconds, unsigned long>;
template class units::base_unit<units::millimeters, double>;
template class units::base_unit<units::volts, double>;
template class units::base_unit<units::percentage, double>;
template class units::base_unit<units::degrees, double>;
template class units::base_unit<units::radians, double>;