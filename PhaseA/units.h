#pragma once
// __DO_NOT_CHANGE_THIS_FILE__
#define MTRN_UNITS_VERSION 1.0

/**
 * \brief The units namespace contains class representing physical units of
 * time, velocity, distance, and angle. Inspired by C++ std::chrono but only
 * provides a very limited amount of std::chrono functionality. Units provide
 * strongly typed representation of time, allowing the compiler to check the
 * parameter passed to a function is correct by checking it type. The time types
 * are implemented using CRTP technique
 */
namespace units
{
/**
 * \brief The units_base template provides common functionality of all units
 * types, including storing value, provide access to the underlying data,
 * provide type defines, provide comparison, addition, and subtraction.
 * \tparam derived_units_name is the class deriving from time_points to
 * implement a specific type of time
 */
template <typename derived_units_name, typename value_t = double>
class base_unit
{
    public:
    /**
     * \brief The unit_type type is the actual units type derived from base_unit
     * class template.
     */
    using unit_type = derived_units_name;

    /**
     * \brief The value_type type is the underlying data type.
     */
    using value_type = value_t;

    private:
    /**
     * \brief value_ is the underlying data storage.
     * represent.
     */
    value_type value_;

    public:
    /**
     * \brief The base_unit constructor
     * \param value is the amount to store.
     */
    explicit base_unit (value_type value) noexcept
        : value_ {value}
    {
    }

    /**
     * \brief The count method converts the unit to a numeric type.
     * \return the raw value of the stored unit.
     */
    value_type count () const
    {
        return value_;
    }

    /**
     * \brief The operator+ adds two units objects of the same type together.
     * \param rhs is the duration to add.
     * \return a new object storing the result.
     */
    auto operator+ (unit_type rhs) const -> unit_type;

    /**
     * \brief The operator- subtract a units object from another object of the
     * same type. \param rhs is amount to subtract. \return a new object storing
     * the result.
     */
    auto operator- (unit_type rhs) const -> unit_type;

    auto operator* (value_type rhs) const -> unit_type;

    auto operator/ (value_type rhs) const -> unit_type;

    // Boiler plate comparator operator, in C++20 can be replaced by spaceship
    // operation

    /**
     * \brief The operator< compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs < rhs
     */
    auto operator< (unit_type rhs) const -> bool;

    /**
     * \brief The operator<= compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs <= rhs
     */
    auto operator<= (unit_type rhs) const -> bool;

    /**
     * \brief The operator> compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs > rhs
     */
    auto operator> (unit_type rhs) const -> bool;

    /**
     * \brief The operator>= compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs >= rhs
     */
    auto operator>= (unit_type rhs) const -> bool;

    /**
     * \brief The operator== compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs == rhs
     */
    auto operator== (unit_type rhs) const -> bool;

    /**
     * \brief The operator!= compares two units of the same type.
     * \param rhs is the right hand side value
     * \return true if lhs != rhs
     */
    auto operator!= (unit_type rhs) const -> bool;
};

/**
 * \brief The microseconds class is a time_point with microseconds time unit.
 */
class microseconds : public base_unit<microseconds, unsigned long>
{
    public:
    using base_unit::base_unit;    // inheriting base constructor

    /**
     * \brief The now method returns the number of microseconds since the
     * program started. Overflow approximately every 70 minutes
     * \return microseconds since start of program
     */
    static auto now () noexcept -> microseconds;
};

/**
 * \brief The milliseconds class is a time_point with milliseconds time unit.
 */
class milliseconds : public base_unit<milliseconds, unsigned long>
{
    public:
    using base_unit::base_unit;    // inheriting base constructor

    /**
     * \brief The now  method returns the number of microseconds since the
     * program started. Overflow approximately every 50 days.
     * \return milliseconds since start of program
     */
    static auto now () noexcept -> milliseconds;
};

/**
 * \brief The sleep_for method pause the program for period of time.
 * \param period is the number of milliseconds to pause the program.
 */
auto sleep_for (milliseconds period) -> void;

/**
 * \brief The sleep_for method pause the program for period of time.
 * \param period is the number of microseconds to pause the program.
 */
auto sleep_for (microseconds period) -> void;

/**
 * \brief The sleep_until pause the program until a specific time since start of
 * program
 * \param wake_time is the time to resume execution in milliseconds.
 */
auto sleep_until (milliseconds wake_time) -> void;

/**
 * \brief The sleep_until pause the program until a specific time since start of
 * program
 * \param wake_time is the time to resume execution in microseconds.
 */
auto sleep_until (microseconds wake_time) -> void;

class millimeters : public base_unit<millimeters, double>
{
    public:
    using base_unit::base_unit;    // inheriting base constructor
};

class volts : public base_unit<volts, double>
{
    public:
    using base_unit::base_unit;    // inheriting base constructor
};

class percentage : public base_unit<percentage, double>
{
    public:
    using base_unit::base_unit;    // inheriting base constructor
};

/**
 * \brief The literals namespace defines milliseconds and microseconds
 * literal for conveniently enter time.
 * To use first include the literals into local scope by:
 * using namespace units::literals;
 * then one milliseconds can be specified using _ms suffix.
 * EG:
 *     auto one_milliseconds = 1_ms;
 * and microseconds as _us.
 */
namespace literals
{
template <typename units_type, typename value_t>
auto units_literals_helper (value_t value) -> units_type
{
    return units_type {static_cast<typename units_type::value_type> (value)};
}

/**
 * \brief The operator ""_ms user defined literal, for easily specifying time in
milliseconds.
*  \param value is the period of time in milliseconds
*  \return value as milliseconds
*/
inline auto operator"" _ms (unsigned long long int value) -> milliseconds
{
    return units_literals_helper<milliseconds> (value);
}

/**
 * \brief The operator ""_us user defined literal, for easily specifying time in
 * milliseconds.
 * \param value is the period of time in microseconds
 * \return value as microseconds
 */
inline auto operator"" _us (unsigned long long int value) -> microseconds
{
    return units_literals_helper<microseconds> (value);
}

/**
 * \brief The operator ""_mm user defined literal, for easily specifying
 * distance in millimeters. \param value is the distance in millimeters \return
 * value as millimeters
 */
inline auto operator""_mm (long double value) -> millimeters
{
    return units_literals_helper<millimeters> (value);
}

inline auto operator""_v (long double value) -> volts
{
    return units_literals_helper<volts> (value);
}

inline auto operator""_percent (long double value) -> percentage
{
    return units_literals_helper<percentage> (value);
}
}    // namespace literals

class degrees;
#undef degrees
#undef radians

class radians : public base_unit<radians, double>
{
    public:
    using base_unit::base_unit;
    explicit operator degrees () const;
};

class degrees : public base_unit<degrees, double>
{
    public:
    using base_unit::base_unit;
    explicit operator radians () const;
};

namespace literals
{
inline auto operator""_deg (long double value) -> degrees
{
    return degrees {static_cast<double> (value)};
}

inline auto operator""_rad (long double value) -> radians
{
    return radians {static_cast<double> (value)};
}
}    // namespace literals
}    // namespace units
