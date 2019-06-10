#pragma once
// __STUDENT_DONT_NEED_TO_UNDERSTAND_THIS_FILE__
// __DO_NOT_CHANGE_THIS_FILE__
#define MTRN_TYPE_TRAITS 1.0

// Implementing some basic c++ type_traits missing from avr-gcc.
// Most code copied from cppreference.com see
// https://en.cppreference.com/w/Cppreference:Copyright/CC-BY-SA
template <class T>
struct remove_reference;
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class T>
struct remove_reference
{
    typedef T type;
};
template <class T>
struct remove_reference<T &>
{
    typedef T type;
};
template <class T>
struct remove_reference<T &&>
{
    typedef T type;
};

template <class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant type;    // using injected-class-name
    constexpr operator value_type () const noexcept
    {
        return value;
    }
    constexpr value_type operator() () const noexcept
    {
        return value;
    }    // since c++14
};
template <bool B>
using bool_constant = integral_constant<bool, B>;
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class T>
struct is_lvalue_reference;
#if GCC_VERSION > 50000
template <class T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
#endif

template <class T>
struct is_lvalue_reference : false_type
{
};
template <class T>
struct is_lvalue_reference<T &> : true_type
{
};

template <class T, class U>
struct is_same : false_type
{
};

template <class T>
struct is_same<T, T> : true_type
{
};

template <bool B, class T = void>
struct enable_if
{
};

template <class T>
struct enable_if<true, T>
{
    typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <template <class...> class Template, class T>
struct is_instantiation : false_type
{
};
template <template <class...> class Template, class... Ts>
struct is_instantiation<Template, Template<Ts...>> : true_type
{
};

template <class a, class u>
struct non_type_parameter;

template <class a, a i, template <a> class u>
struct non_type_parameter<a, u<i>>
{
    static constexpr a value = i;
};

// template <class a, class u>
// constexpr bool non_type_parameter_v = non_type_parameter<a, u>::value;

template <typename tag, class non_type_arg_t, template <non_type_arg_t> class t>
using check_tag
    = is_same<tag, t<non_type_parameter<non_type_arg_t, tag>::value>>;

// template<class T, template <class> class U>
// constexpr bool is_instance_v<U<T>, U> = is_instance<U<T>, U>::value;

template <class T>
T && forward (typename remove_reference<T>::type & t) noexcept
{
    return static_cast<T &&> (t);
}

template <class T>
T && forward (typename remove_reference<T>::type && t) noexcept
{
    static_assert (!is_lvalue_reference<T>::value,
        "Can not forward an rvalue as an lvalue.");
    return static_cast<T &&> (t);
}

//#pragma once
//
// template <class T, T v>
// struct integral_constant
//{
//    static constexpr T value = v;
//    typedef T value_type;
//    typedef integral_constant type;    // using injected-class-name
//    constexpr operator value_type () const noexcept
//    {
//        return value;
//    }
//    constexpr value_type operator() () const noexcept
//    {
//        return value;
//    }    // since c++14
//};
//
// using true_type = integral_constant<bool, true>;
// using false_type = integral_constant<bool, false>;
//
// template <class T, class U>
// struct is_same : false_type
//{
//};
//
// template <class T>
// struct is_same<T, T> : true_type
//{
//};
//
// template <bool B, class T = void>
// struct enable_if
//{
//};
//
// template <class T>
// struct enable_if<true, T>
//{
//    typedef T type;
//};
//
// template< bool B, class T = void >
// using enable_if_t = typename enable_if<B,T>::type;
