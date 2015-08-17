#pragma once

#include "World.hpp"
#include "AvdeccString.hpp"
#include "AvdeccEncoding.hpp"

namespace ControlPlane
{

/// \brief powers_of_ten
///
/// The powers of ten values as float from 1e-12 to 1e12 inclusive
///
extern float powers_of_ten[25];

///
///
/// \brief integer_pow10
///
/// Template function to get the value of an integer power of ten
/// Works with integer types and floating point types.
///
template <typename T1, typename T2>
T1 integer_pow10( T2 exponent )
{
    T1 r = 0;
    int e = int( exponent ) + 12;
    if ( e >= 0 && e <= 25 )
    {
        r = powers_of_ten[e];
    }
    return r;
}

///
/// \brief getEncodingMultiplier
///
/// Get the multiplier required to convert an unencoded value
/// to an encoded value.
///
/// If the multiplier_power is <=0 this method returns 1
///
/// \return The value to multiply with
///
template <typename T>
T getEncodingMultiplier( int8_t multiplier_power )
{
    T r = 1;
    if ( multiplier_power < 0 )
    {
        r = integer_pow10<T>( -multiplier_power );
    }
    return r;
}

///
/// \brief getEncodingDivider
///
/// Get the divider required to convert an unencoded value
/// to an encoded value
///
/// If the multiplier_power is >=0 this method returns 1
///
/// \return the value to divide with
///
template <typename T>
T getEncodingDivider( int8_t multiplier_power )
{
    T r = 1;
    if ( multiplier_power > 0 )
    {
        r = integer_pow10<T>( multiplier_power );
    }
    return r;
}

///
/// \brief getDecodingMultiplier
///
/// Get the multiplier required to convert an encoded value
/// to an unencoded value.
///
/// If the multiplier_power is >=0 this method returns 1
///
/// \return The value to multiply with
///
template <typename T>
T getDecodingMultiplier( int8_t multiplier_power )
{
    T r = 1;
    if ( multiplier_power > 0 )
    {
        r = integer_pow10<T>( multiplier_power );
    }
    return r;
}

///
/// \brief getDecodingDivider
///
/// Get the divider required to convert an encoded value
/// to an unencoded value
///
/// If the multiplier_power is <=0 this method returns 1
///
/// \return the value to divide with
///
template <typename T>
T getDecodingDivider( int8_t multiplier_power )
{
    T r = 1;
    if ( multiplier_power < 0 )
    {
        r = integer_pow10<T>( -multiplier_power );
    }
    return r;
}

///
/// \brief The EncodingType enum
///
/// Enumeration of possible encoding types to describe transport encoding and
/// storage encoding
///
enum class EncodingType : uint8_t
{
    ENCODING_INT8,
    ENCODING_UINT8,
    ENCODING_INT16,
    ENCODING_UINT16,
    ENCODING_INT32,
    ENCODING_UINT32,
    ENCODING_INT64,
    ENCODING_UINT64,
    ENCODING_FLOAT,
    ENCODING_DOUBLE,
    ENCODING_STRING406,
    ENCODING_STRING64
};

///
/// \brief EncodingTypeFor template traits class
///
/// Specialized for the C++ types which map to an EncodingType enumeration value
/// So that code can look up the enumeration value for a static type
///
template <typename T>
struct EncodingTypeFor
{
};

template <>
struct EncodingTypeFor<bool>
{
    using type = bool;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_UINT8; }
};

template <>
struct EncodingTypeFor<int8_t>
{
    using type = int8_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_INT8; }
};

template <>
struct EncodingTypeFor<uint8_t>
{
    using type = uint8_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_UINT8; }
};

template <>
struct EncodingTypeFor<int16_t>
{
    using type = int16_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_INT16; }
};

template <>
struct EncodingTypeFor<uint16_t>
{
    using type = uint16_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_UINT16; }
};

template <>
struct EncodingTypeFor<int32_t>
{
    using type = int32_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_INT32; }
};

template <>
struct EncodingTypeFor<uint32_t>
{
    using type = uint32_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_UINT32; }
};

template <>
struct EncodingTypeFor<int64_t>
{
    using type = int64_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_INT64; }
};

template <>
struct EncodingTypeFor<uint64_t>
{
    using type = uint64_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_UINT64; }
};

template <>
struct EncodingTypeFor<float>
{
    using type = float;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_FLOAT; }
};

template <>
struct EncodingTypeFor<double>
{
    using type = uint64_t;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_DOUBLE; }
};

template <>
struct EncodingTypeFor<string>
{
    using type = string;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_STRING406; }
};

template <>
struct EncodingTypeFor<AvdeccControlString>
{
    using type = string;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_STRING406; }
};

template <>
struct EncodingTypeFor<AvdeccNameString>
{
    using type = string;
    static EncodingType getEncodingType() { return EncodingType::ENCODING_STRING64; }
};
}
