#pragma once

#include "World.hpp"
#include "AvdeccString.hpp"
#include "AvdeccUnits.hpp"
#include "AvdeccEncoding.hpp"

namespace ControlPlane
{

///
/// \brief The RangedValueBase class
///
/// Base class for the RangedValue
/// Pure-abstract interfaces to allow querying of meta data and values
/// and setting of values
///
class RangedValueBase
{
  public:
    virtual ~RangedValueBase() {}

    ///
    /// \brief getUnitsCode
    ///
    /// \return The UnitsCode for this value
    ///
    virtual UnitsCode getUnitsCode() const = 0;

    ///
    /// \brief getStorageType
    /// \return The EncodingType used for the storage of this value
    ///
    virtual EncodingType getStorageType() const = 0;

    ///
    /// \brief getEncodingType
    /// \return The EncodingType used for the encoded transport of this value
    ///
    virtual EncodingType getEncodingType() const = 0;

    ///
    /// \brief isReadOnly
    /// \return true if the value is not to be set by the user
    ///
    virtual bool isReadOnly() const { return false; }

    ///
    /// \brief setUnencodedValueString
    ///
    /// If storage type and encoding type are both string, then set
    /// the string value
    ///
    /// \param v The new string value
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueString( string const &v, bool force = false ) = 0;

    ///
    /// \brief setUnencodedValueBool
    ///
    /// Set the unencoded value from a bool
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueBool( bool v, bool force = false ) = 0;

    ///
    /// \brief setUnencodedValueFloat
    ///
    /// Set the unencoded value from a float
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueFloat( float v, bool force = false ) = 0;

    ///
    /// \brief setUnencodedValueDouble
    ///
    /// Set the unencoded value from a double
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueDouble( double v, bool force = false ) = 0;

    ///
    /// \brief setUnencodedValueInt64
    ///
    /// Set the unencoded value from an int64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueInt64( int64_t v, bool force = false ) = 0;

    ///
    /// \brief setUnencodedValueUInt64
    ///
    /// Set the unencoded value from an uint64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    virtual bool setUnencodedValueUInt64( uint64_t v, bool force = false ) = 0;

    ///
    /// \brief getUnencodedValueString
    ///
    /// Get the unencoded value string. If the
    /// storage type and encoding type is string, then
    /// it returns the value. If the storage type is not
    /// string then it returns the textual representation
    /// optionally with units
    ///
    /// \return the string representation of the unencoded value
    ///
    virtual string getUnencodedValueString( bool enable_units = true ) const = 0;

    virtual string getUnencodedMinimumString( bool enable_units = true ) const = 0;

    virtual string getUnencodedMaximumString( bool enable_units = true ) const = 0;

    virtual string getUnencodedDefaultString( bool enable_units = true ) const = 0;

    virtual string getUnencodedStepString( bool enable_units = true ) const = 0;

    ///
    /// \brief getUnencodedValueBool
    /// \return the unencoded value as a bool
    ///
    virtual bool getUnencodedValueBool() const = 0;

    ///
    /// \brief getUnencodedValueFloat
    /// \return the unencoded value as a float
    ///
    virtual float getUnencodedValueFloat() const = 0;

    ///
    /// \brief getUnencodedValueDouble
    /// \return the unencoded value as a double
    ///
    virtual double getUnencodedValueDouble() const = 0;

    ///
    /// \brief getUnencodedValueInt64
    /// \return the unencoded value as an int64_t
    ///
    virtual int64_t getUnencodedValueInt64() const = 0;

    ///
    /// \brief getUnencodedValueUInt64
    /// \return the unencoded value as a uint64_t
    ///
    virtual uint64_t getUnencodedValueUInt64() const = 0;

    virtual float getUnencodedMinimumFloat() const = 0;

    virtual uint64_t getUnencodedMinimumUInt64() const = 0;

    virtual float getUnencodedMaximumFloat() const = 0;

    virtual uint64_t getUnencodedMaximumUInt64() const = 0;

    ///
    /// \brief incValue
    ///
    /// Increment the value by the step value
    ///
    /// \return true if the value changed
    ///
    virtual bool incValue() = 0;

    ///
    /// \brief decValue
    ///
    /// Decrement the value by the step value
    ///
    /// \return true if the value changed
    ///
    virtual bool decValue() = 0;

    virtual void getEncodedValueAvdeccString( AvdeccString *storage ) const = 0;
    virtual int8_t getEncodedValueInt8() const = 0;
    virtual uint8_t getEncodedValueUInt8() const = 0;
    virtual int16_t getEncodedValueInt16() const = 0;
    virtual uint16_t getEncodedValueUInt16() const = 0;
    virtual int32_t getEncodedValueInt32() const = 0;
    virtual uint32_t getEncodedValueUInt32() const = 0;
    virtual int64_t getEncodedValueInt64() const = 0;
    virtual uint64_t getEncodedValueUInt64() const = 0;
    virtual float getEncodedValueFloat() const = 0;
    virtual double getEncodedValueDouble() const = 0;

    virtual void setFromEncodedValueAvdeccString( const AvdeccString *storage ) = 0;
    virtual bool setFromEncodedValueInt8( int8_t v ) = 0;
    virtual bool setFromEncodedValueUInt8( uint8_t v ) = 0;
    virtual bool setFromEncodedValueInt16( int16_t v ) = 0;
    virtual bool setFromEncodedValueUInt16( uint16_t v ) = 0;
    virtual bool setFromEncodedValueInt32( int32_t v ) = 0;
    virtual bool setFromEncodedValueUInt32( uint32_t v ) = 0;
    virtual bool setFromEncodedValueInt64( int64_t v ) = 0;
    virtual bool setFromEncodedValueUInt64( uint64_t v ) = 0;
    virtual bool setFromEncodedValueFloat( float v ) = 0;
    virtual bool setFromEncodedValueDouble( double v ) = 0;

    virtual bool setFromEncodedValueWithClampInt8( int8_t v ) = 0;
    virtual bool setFromEncodedValueWithClampUInt8( uint8_t v ) = 0;
    virtual bool setFromEncodedValueWithClampInt16( int16_t v ) = 0;
    virtual bool setFromEncodedValueWithClampUInt16( uint16_t v ) = 0;
    virtual bool setFromEncodedValueWithClampInt32( int32_t v ) = 0;
    virtual bool setFromEncodedValueWithClampUInt32( uint32_t v ) = 0;
    virtual bool setFromEncodedValueWithClampInt64( int64_t v ) = 0;
    virtual bool setFromEncodedValueWithClampUInt64( uint64_t v ) = 0;
    virtual bool setFromEncodedValueWithClampFloat( float v ) = 0;
    virtual bool setFromEncodedValueWithClampDouble( double v ) = 0;

    ///
    /// \brief getEncodedMinValue
    /// \return The encoded minimum value
    ///
    virtual int64_t getEncodedMinValue() const = 0;

    ///
    /// \brief getEncodedMaxValue
    /// \return The encoded maximum value
    ///
    virtual int64_t getEncodedMaxValue() const = 0;

    ///
    /// \brief getEncodedStepValue
    /// \return The encoded step value
    ///
    virtual int64_t getEncodedStepValue() const = 0;

    ///
    /// \brief getEncodedDefaultValue
    /// \return the encoded default value
    ///
    virtual int64_t getEncodedDefaultValue() const = 0;

    ///
    /// \brief getEncodingMultiplierPower
    /// \return the power of 10 used for encoding
    ///
    virtual int8_t getEncodingMultiplierPower() const = 0;

    ///
    /// \brief getUnitsSuffix
    ///
    /// Get the units suffix
    ///
    /// \return C string containing UTF8 suffix for the SI Units used
    ///
    virtual const char *getUnitsSuffix() const = 0;

    bool setUnencodedValue( bool v, bool force = false ) { return setUnencodedValueBool( v, false ); }

    bool setUnencodedValue( double v, bool force = false ) { return setUnencodedValueDouble( v, false ); }

    bool setUnencodedValue( float v, bool force = false ) { return setUnencodedValueFloat( v, false ); }

    bool setUnencodedValue( int8_t v, bool force = false ) { return setUnencodedValueInt64( v, false ); }

    bool setUnencodedValue( uint8_t v, bool force = false ) { return setUnencodedValueUInt64( v, false ); }

    bool setUnencodedValue( int16_t v, bool force = false ) { return setUnencodedValueInt64( v, false ); }

    bool setUnencodedValue( uint16_t v, bool force = false ) { return setUnencodedValueUInt64( v, false ); }

    bool setUnencodedValue( int32_t v, bool force = false ) { return setUnencodedValueInt64( v, false ); }

    bool setUnencodedValue( uint32_t v, bool force = false ) { return setUnencodedValueUInt64( v, false ); }

    bool setUnencodedValue( int64_t v, bool force = false ) { return setUnencodedValueInt64( v, false ); }

    bool setUnencodedValue( uint64_t v, bool force = false ) { return setUnencodedValueUInt64( v, false ); }

    void getUnencodedValue( bool *v ) const { *v = getUnencodedValueBool(); }

    void getUnencodedValue( double *v ) const { *v = getUnencodedValueDouble(); }

    void getUnencodedValue( float *v ) const { *v = getUnencodedValueFloat(); }

    void getUnencodedValue( int64_t *v ) const { *v = getUnencodedValueInt64(); }

    void getUnencodedValue( string *v ) const { *v = getUnencodedValueString( false ); }

    void getUnencodedValue( uint64_t *v ) const { *v = getUnencodedValueUInt64(); }
};

/// \brief the RangedValue class
///
/// A holder for a limited range value with options for defaults and inc/dec
/// step size
/// and with the capability to encode/decode into a fixed point integer type
///
template <UnitsCode UnitsValue,
          int64_t MinValue,
          int64_t MaxValue,
          int64_t DefaultValue = 0,
          int64_t StepValue = 1,
          int MultiplierPowerValue = 0,
          typename EncodedT = int32_t,
          typename ValueT = float>
class RangedValue : public RangedValueBase
{
    static_assert( MinValue <= DefaultValue, "MinValue is not less than or equal to DefaultValue" );
    static_assert( DefaultValue <= MaxValue, "DefaultValue is not less than or equal to MaxValue" );

  public:
    typedef ValueT value_type;
    typedef EncodedT encoded_type;

    static const UnitsCode units = UnitsValue;
    static const int64_t min_value = MinValue;
    static const int64_t max_value = MaxValue;
    static const int64_t step_value = StepValue;
    static const int64_t default_value = DefaultValue;
    static const int multiplier_power = MultiplierPowerValue;

    ///
    /// \brief Value Constructor
    ///
    /// Initialize to the default value
    ///
    RangedValue() { setDefault(); }

    ///
    /// \brief Value implicit Constructor
    ///
    /// Initialize based on value. May throw range_error if the value is out of
    /// range
    ///
    /// \param v value
    ///
    RangedValue( value_type v ) { setValue( v ); }

    ///
    /// \brief Value
    ///
    /// Copy constructor is faster since the m_value is already validated
    ///
    /// \param v Source Value object
    ///
    RangedValue( RangedValue const &v ) : m_value( v.m_value ) {}

    ///
    /// \brief operator =
    ///
    /// Assignment operator is faster since the m_value is already validated
    ///
    /// \param v Source Value object
    /// \return  *this
    ///
    RangedValue &operator=( RangedValue const &v )
    {
        m_value = v.m_value;
        return *this;
    }

    ///
    /// \brief operator value_type
    ///
    operator value_type() const { return m_value; }

    bool isReadOnly() const override { return false; }

    ///
    /// \brief setDefault
    ///
    /// Sets the value to the default value
    ///
    /// \return true if the value changed
    ///
    bool setDefault() { return setValue( getDefaultValue() ); }

    ///
    /// \brief setValue
    ///
    /// Set the value to a new value.
    ///
    /// throws range_error if the requested value is out of range
    ///
    /// \param v the requested value
    /// \return true if the value changed
    ///
    bool setValue( value_type v, bool force = false )
    {
        bool r = false;

        if ( !force )
        {
            value_type min = getMinValue();
            value_type max = getMaxValue();
            if ( v < min )
            {
                throw std::range_error( "setValue() too small" );
            }
            if ( v > max )
            {
                throw std::range_error( "setValue() too large" );
            }
            if ( m_value != v )
            {
                m_value = v;
                r = true;
            }
        }
        else
        {
            if ( m_value != v )
            {
                m_value = v;
                r = true;
            }
        }
        return r;
    }

    ///
    /// \brief setValueWithClamp
    ///
    /// Set the value to a new value.
    ///
    /// clamps the value to min or max if the requested value is out of range
    ///
    /// \param v the requested value
    /// \return true if the value changed
    ///
    bool setValueWithClamp( value_type v )
    {
        bool r = false;
        if ( v < getMinValue() )
        {
            v = getMinValue();
        }
        else if ( v > getMaxValue() )
        {
            v = getMaxValue();
        }

        if ( m_value != v )
        {
            m_value = v;
            r = true;
        }
        return r;
    }

    EncodingType getStorageType() const override { return EncodingTypeFor<ValueT>::getEncodingType(); }

    EncodingType getEncodingType() const override { return EncodingTypeFor<EncodedT>::getEncodingType(); }

    bool setUnencodedValueString( string const &v, bool force ) override
    {
        std::istringstream buf( v );
        value_type actual;
        buf >> actual;
        return setUnencodedValue( actual );
    }

    bool setUnencodedValueBool( bool v, bool force ) override
    {
        uint8_t v8 = v ? 255 : 0;

        return setValue( value_type( v8 ), force );
    }

    bool setUnencodedValueFloat( float v, bool force ) override { return setValue( v, force ); }

    bool setUnencodedValueDouble( double v, bool force ) override { return setValue( (value_type)v, force ); }

    bool setUnencodedValueInt64( int64_t v, bool force ) override { return setValue( (value_type)v, force ); }

    bool setUnencodedValueUInt64( uint64_t v, bool force ) override { return setValue( (value_type)v, force ); }

    string getUnencodedValueString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    string getUnencodedMinimumString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getMinValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    string getUnencodedMaximumString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getMaxValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    string getUnencodedStepString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getStepValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    string getUnencodedDefaultString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getDefaultValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    bool getUnencodedValueBool() const override { return m_value != value_type(); }

    float getUnencodedValueFloat() const override { return (float)m_value; }

    double getUnencodedValueDouble() const override { return (double)m_value; }

    int64_t getUnencodedValueInt64() const override { return (int64_t)m_value; }

    uint64_t getUnencodedValueUInt64() const override { return (uint64_t)m_value; }

    float getUnencodedMinimumFloat() const override { return (float)getMinValue(); }

    uint64_t getUnencodedMinimumUInt64() const override { return (uint64_t)getMinValue(); }

    float getUnencodedMaximumFloat() const override { return (float)getMaxValue(); }

    uint64_t getUnencodedMaximumUInt64() const override { return (uint64_t)getMaxValue(); }

    ///
    /// \brief incValue
    ///
    /// Increment the current value by the step size.
    /// Will not increment past the max value
    ///
    /// \return true if the value changed
    ///
    bool incValue() override
    {
        bool r = false;
        value_type new_value = m_value + getStepValue();
        if ( new_value > getMaxValue() )
        {
            new_value = getMaxValue();
        }
        if ( m_value != new_value )
        {
            m_value = new_value;
            r = true;
        }
        return r;
    }

    ///
    /// \brief decValue
    ///
    /// Decrement the current value by the step size.
    /// Will not decrement past the min value
    ///
    /// \return true if the value changed
    ///
    bool decValue() override
    {
        bool r = false;
        value_type new_value = m_value - getStepValue();
        if ( new_value > getMaxValue() )
        {
            new_value = getMaxValue();
        }
        if ( m_value != new_value )
        {
            m_value = new_value;
            r = true;
        }
        return r;
    }

    ///
    /// \brief getValue
    ///
    /// Get the current value
    ///
    /// \return the value
    ///
    value_type getValue() const { return m_value; }

    ///
    /// \brief getMinValue
    ///
    /// Get the minimum value
    ///
    /// \return the minimum value
    ///
    value_type getMinValue() const
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( min_value ) * decoding_multiplier / decoding_divider;
        return v;
    }

    ///
    /// \brief getMaxValue
    ///
    /// Get the maximum value
    ///
    /// \return the maximum value
    ///
    value_type getMaxValue() const
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( max_value ) * decoding_multiplier / decoding_divider;
        return v;
    }

    ///
    /// \brief getDefaultValue
    ///
    /// Get the default value
    ///
    /// \return the default value
    ///
    value_type getDefaultValue() const
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( default_value ) * decoding_multiplier / decoding_divider;
        return v;
    }

    ///
    /// \brief getStepValue
    ///
    /// Get the step value
    ///
    /// \return the step value
    ///
    value_type getStepValue() const
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( step_value ) * decoding_multiplier / decoding_divider;
        return v;
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
    value_type getEncodingMultiplier() const
    {
        value_type r = 1;
        if ( multiplier_power < 0 )
        {
            r = integer_pow10<value_type>( -multiplier_power );
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
    value_type getEncodingDivider() const
    {
        value_type r = 1;
        if ( multiplier_power > 0 )
        {
            r = integer_pow10<value_type>( multiplier_power );
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
    value_type getDecodingMultiplier() const
    {
        value_type r = 1;
        if ( multiplier_power > 0 )
        {
            r = integer_pow10<value_type>( multiplier_power );
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
    value_type getDecodingDivider() const
    {
        value_type r = 1;
        if ( multiplier_power < 0 )
        {
            r = integer_pow10<value_type>( -multiplier_power );
        }
        return r;
    }

    ///
    /// Default template function to help round integer types
    ///
    template <typename T>
    static T valueRound( T v )
    {
        return v;
    }

    ///
    /// \brief valueRound
    ///
    /// overload to round float types
    ///
    /// \param v value to round
    /// \return the rounded value
    ///
    static float valueRound( float v ) { return roundf( v ); }

    ///
    /// \brief valueRound
    ///
    /// overload to round double types
    ///
    /// \param v the value to round
    /// \return  the rounded value
    ///
    static double valueRound( double v ) { return round( v ); }

    ///
    /// \brief getEncodedValue
    ///
    /// Convert the current value to an encoded value
    ///
    /// If the result type is integer, then the result is rounded first.
    ///
    /// If the result type does not have enough bits to hold the full range
    /// of the encoded value, this function throws domain_error
    ///
    /// \param dest Pointer to the result
    ///
    template <typename T>
    void getEncodedValue( T *dest ) const
    {
        value_type encoding_multiplier = getEncodingMultiplier();
        value_type encoding_divider = getEncodingDivider();
        value_type v = ( getValue() * encoding_multiplier / encoding_divider );
        value_type rounded_v;

        if ( !std::is_floating_point<T>::value && std::is_floating_point<value_type>::value )
        {
            rounded_v = valueRound( v );
        }
        else
        {
            rounded_v = v;
        }
        if ( getMaxValue() * encoding_multiplier > (value_type)std::numeric_limits<T>::max() )
        {
            throw std::domain_error( "Max Value too large for encoding" );
        }
        if ( getMinValue() * encoding_multiplier < (value_type)std::numeric_limits<T>::min() )
        {
            throw std::domain_error( "Min Value too small for encoding" );
        }
        *dest = static_cast<T>( rounded_v );
    }

    void getEncodedValueAvdeccString( AvdeccString *storage ) const override
    {
        string s = getUnencodedValueString( false );

        storage->set( s );
    }

    int8_t getEncodedValueInt8() const override
    {
        int8_t v;
        getEncodedValue( &v );
        return v;
    }

    uint8_t getEncodedValueUInt8() const override
    {
        uint8_t v;
        getEncodedValue( &v );
        return v;
    }

    int16_t getEncodedValueInt16() const override
    {
        int16_t v;
        getEncodedValue( &v );
        return v;
    }

    uint16_t getEncodedValueUInt16() const override
    {
        uint16_t v;
        getEncodedValue( &v );
        return v;
    }

    int32_t getEncodedValueInt32() const override
    {
        int32_t v;
        getEncodedValue( &v );
        return v;
    }

    uint32_t getEncodedValueUInt32() const override
    {
        uint32_t v;
        getEncodedValue( &v );
        return v;
    }

    int64_t getEncodedValueInt64() const override
    {
        int64_t v;
        getEncodedValue( &v );
        return v;
    }

    uint64_t getEncodedValueUInt64() const override
    {
        uint64_t v;
        getEncodedValue( &v );
        return v;
    }

    float getEncodedValueFloat() const override
    {
        float v;
        getEncodedValue( &v );
        return v;
    }

    double getEncodedValueDouble() const override
    {
        double v;
        getEncodedValue( &v );
        return v;
    }

    ///
    /// \brief setFromEncodedValue
    ///
    /// If the encoded_v is out of range, this function will throw range_error
    ///
    /// \param encoded_v The encoded value to use to set the value to
    ///
    /// \return true if the value changed
    template <typename T>
    bool setFromEncodedValue( T encoded_v )
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( encoded_v ) * decoding_multiplier / decoding_divider;
        return setValue( v );
    }

    bool setFromEncodedValue( bool encoded_v ) { return setValue( encoded_v ); }

    void setFromEncodedValueAvdeccString( const AvdeccString *storage ) override
    {
        string s = storage->get();
        std::istringstream buf( s );
        buf >> m_value;
    }

    bool setFromEncodedValueInt8( int8_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt8( uint8_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt16( int16_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt16( uint16_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt32( int32_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt32( uint32_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt64( int64_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt64( uint64_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueFloat( float v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueDouble( double v ) override { return setFromEncodedValue( v ); }

    ///
    /// \brief setFromEncodedValueWithClamp
    ///
    /// If the encoded_v is out of range, this function will clamp the
    /// value to the appropriate range
    ///
    /// \param encoded_v The encoded value to use to set the value to
    ///
    /// \return true if the value changed
    template <typename T>
    bool setFromEncodedValueWithClamp( T encoded_v )
    {
        value_type decoding_multiplier = getDecodingMultiplier();
        value_type decoding_divider = getDecodingDivider();
        value_type v = value_type( encoded_v ) * decoding_multiplier / decoding_divider;
        return setValueWithClamp( v );
    }

    bool setFromEncodedValueWithClampInt8( int8_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt8( uint8_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt16( int16_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt16( uint16_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt32( int32_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt32( uint32_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt64( int64_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt64( uint64_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampFloat( float v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampDouble( double v ) override { return setFromEncodedValueWithClamp( v ); }

    ///
    /// \brief getUnitsCode
    ///
    /// Get the units code for this value
    ///
    /// \return The Units enumeration value
    ///
    UnitsCode getUnitsCode() const override { return UnitsValue; }

    ///
    /// \brief getEncodedMinValue
    /// \return The encoded minimum value
    ///
    int64_t getEncodedMinValue() const override { return min_value; }

    ///
    /// \brief getEncodedMaxValue
    /// \return The encoded maximum value
    ///
    int64_t getEncodedMaxValue() const override { return max_value; }

    ///
    /// \brief getEncodedStepValue
    /// \return The encoded step value
    ///
    int64_t getEncodedStepValue() const override { return step_value; }

    ///
    /// \brief getEncodedDefaultValue
    /// \return the encoded default value
    ///
    int64_t getEncodedDefaultValue() const override { return default_value; }

    ///
    /// \brief getEncodingMultiplierPower
    /// \return the power of 10 used for encoding
    ///
    int8_t getEncodingMultiplierPower() const override { return multiplier_power; }

    const char *getUnitsSuffix() const override { return getAvdeccUnitsSuffix( getUnitsCode() ); }

  private:
    ///
    /// \brief m_value
    ///
    /// The actual non-encoded value
    ///
    value_type m_value;
};

template <UnitsCode UnitsValue,
          int64_t MinValue,
          int64_t MaxValue,
          int64_t DefaultValue,
          int64_t StepValue,
          int MultiplierPowerValue,
          typename EncodedT>
class RangedValue<UnitsValue, MinValue, MaxValue, DefaultValue, StepValue, MultiplierPowerValue, EncodedT, string>
    : public RangedValueBase
{
    static_assert( MinValue <= DefaultValue, "MinValue is not less than or equal to DefaultValue" );
    static_assert( DefaultValue <= MaxValue, "DefaultValue is not less than or equal to MaxValue" );

  public:
    typedef string value_type;
    typedef EncodedT encoded_type;

    static const UnitsCode units = UnitsValue;
    static const int64_t min_value = MinValue;
    static const int64_t max_value = MaxValue;
    static const int64_t step_value = StepValue;
    static const int64_t default_value = DefaultValue;
    static const int multiplier_power = MultiplierPowerValue;

    ///
    /// \brief Value Constructor
    ///
    /// Initialize to the default value
    ///
    RangedValue() { setDefault(); }

    ///
    /// \brief Value implicit Constructor
    ///
    /// Initialize based on value. May throw range_error if the value is out of
    /// range
    ///
    /// \param v value
    ///
    RangedValue( value_type v, bool force = false ) { setValue( v, force ); }

    ///
    /// \brief Value
    ///
    /// Copy constructor is faster since the m_value is already validated
    ///
    /// \param v Source Value object
    ///
    RangedValue( RangedValue const &v ) : m_value( v.m_value ) {}

    ///
    /// \brief operator =
    ///
    /// Assignment operator is faster since the m_value is already validated
    ///
    /// \param v Source Value object
    /// \return  *this
    ///
    RangedValue &operator=( RangedValue const &v )
    {
        m_value = v.m_value;
        return *this;
    }

    ///
    /// \brief operator value_type
    ///
    operator value_type() const { return m_value; }

    bool isReadOnly() const override { return false; }

    ///
    /// \brief setDefault
    ///
    /// Sets the value to the default value
    ///
    /// \return true if the value changed
    ///
    bool setDefault() { return setValue( getDefaultValue() ); }

    ///
    /// \brief setValue
    ///
    /// Set the value to a new value.
    ///
    /// throws range_error if the requested value is out of range
    ///
    /// \param v the requested value
    /// \return true if the value changed
    ///
    bool setValue( value_type v, bool force = false )
    {
        bool r = false;
        if ( m_value != v )
        {
            m_value = v;
            r = true;
        }
        return r;
    }

    ///
    /// \brief setValueWithClamp
    ///
    /// Set the value to a new value.
    ///
    /// clamps the value to min or max if the requested value is out of range
    ///
    /// \param v the requested value
    /// \return true if the value changed
    ///
    bool setValueWithClamp( value_type v )
    {
        bool r = false;
        if ( m_value != v )
        {
            m_value = v;
            r = true;
        }
        return r;
    }

    EncodingType getStorageType() const override { return EncodingTypeFor<value_type>::getEncodingType(); }

    EncodingType getEncodingType() const override { return EncodingTypeFor<encoded_type>::getEncodingType(); }

    bool setUnencodedValueString( string const &v, bool force ) override { return setValue( v, force ); }

    bool setUnencodedValueBool( bool v, bool force = false ) override { return setValue( v ? "true" : "false" ); }

    bool setUnencodedValueFloat( float v, bool force = false ) override
    {
        std::ostringstream buf;
        buf << v;
        return setValue( buf.str() );
    }

    bool setUnencodedValueDouble( double v, bool force = false ) override
    {
        std::ostringstream buf;
        buf << v;
        return setValue( buf.str() );
    }

    bool setUnencodedValueInt64( int64_t v, bool force = false ) override
    {
        std::ostringstream buf;
        buf << v;
        return setValue( buf.str() );
    }

    bool setUnencodedValueUInt64( uint64_t v, bool force = false ) override
    {
        std::ostringstream buf;
        buf << v;
        return setValue( buf.str() );
    }

    string getUnencodedValueString( bool enable_units ) const override
    {
        std::ostringstream buf;
        buf << getValue();
        if ( enable_units )
        {
            const char *suffix = getAvdeccUnitsSuffix( units );
            if ( suffix && *suffix )
            {
                buf << " " << suffix;
            }
        }
        return buf.str();
    }

    string getUnencodedMinimumString( bool enable_units ) const override { return ""; }

    string getUnencodedMaximumString( bool enable_units ) const override { return ""; }

    string getUnencodedDefaultString( bool enable_units ) const override { return ""; }

    string getUnencodedStepString( bool enable_units ) const override { return ""; }

    bool getUnencodedValueBool() const override { return m_value == "true" ? true : false; }

    float getUnencodedValueFloat() const override
    {
        std::istringstream buf( m_value );
        float v;
        buf >> v;
        return v;
    }

    double getUnencodedValueDouble() const override
    {
        std::istringstream buf( m_value );
        double v;
        buf >> v;
        return v;
    }

    int64_t getUnencodedValueInt64() const override
    {
        std::istringstream buf( m_value );
        int64_t v;
        buf >> v;
        return v;
    }

    uint64_t getUnencodedValueUInt64() const override
    {
        std::istringstream buf( m_value );
        uint64_t v;
        buf >> v;
        return v;
    }

    float getUnencodedMinimumFloat() const override { return 0.0f; }

    uint64_t getUnencodedMinimumUInt64() const override { return 0; }

    float getUnencodedMaximumFloat() const override { return 0.0f; }

    uint64_t getUnencodedMaximumUInt64() const override { return 0; }

    ///
    /// \brief incValue
    ///
    /// Increment the current value by the step size.
    /// Will not increment past the max value
    ///
    /// \return true if the value changed
    ///
    bool incValue() override { return false; }

    ///
    /// \brief decValue
    ///
    /// Decrement the current value by the step size.
    /// Will not decrement past the min value
    ///
    /// \return true if the value changed
    ///
    bool decValue() override { return false; }

    ///
    /// \brief getValue
    ///
    /// Get the current value
    ///
    /// \return the value
    ///
    value_type getValue() const { return m_value; }

    ///
    /// \brief getMinValue
    ///
    /// Get the minimum value
    ///
    /// \return the minimum value
    ///
    value_type getMinValue() const { return ""; }

    ///
    /// \brief getMaxValue
    ///
    /// Get the maximum value
    ///
    /// \return the maximum value
    ///
    value_type getMaxValue() const { return ""; }

    ///
    /// \brief getDefaultValue
    ///
    /// Get the default value
    ///
    /// \return the default value
    ///
    value_type getDefaultValue() const { return ""; }

    ///
    /// \brief getStepValue
    ///
    /// Get the step value
    ///
    /// \return the step value
    ///
    value_type getStepValue() const { return ""; }

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
    value_type getEncodingMultiplier() const { return ""; }

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
    value_type getEncodingDivider() const { return ""; }

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
    value_type getDecodingMultiplier() const { return ""; }

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
    value_type getDecodingDivider() const { return ""; }

    ///
    /// Default template function to help round integer types
    ///
    template <typename T>
    static T valueRound( T v )
    {
        return v;
    }

    ///
    /// \brief valueRound
    ///
    /// overload to round float types
    ///
    /// \param v value to round
    /// \return the rounded value
    ///
    static float valueRound( float v ) { return roundf( v ); }

    ///
    /// \brief valueRound
    ///
    /// overload to round double types
    ///
    /// \param v the value to round
    /// \return  the rounded value
    ///
    static double valueRound( double v ) { return round( v ); }

    ///
    /// \brief getEncodedValue
    ///
    /// Convert the current value to an encoded value
    ///
    /// If the result type is integer, then the result is rounded first.
    ///
    /// If the result type does not have enough bits to hold the full range
    /// of the encoded value, this function throws domain_error
    ///
    /// \param dest Pointer to the result
    ///
    template <typename T>
    void getEncodedValue( T *dest ) const
    {
        std::istringstream buf( m_value );
        buf >> *dest;
    }

    void getEncodedValueAvdeccString( AvdeccString *storage ) const override { storage->set( m_value ); }

    int8_t getEncodedValueInt8() const override
    {
        int8_t v;
        getEncodedValue( &v );
        return v;
    }

    uint8_t getEncodedValueUInt8() const override
    {
        uint8_t v;
        getEncodedValue( &v );
        return v;
    }

    int16_t getEncodedValueInt16() const override
    {
        int16_t v;
        getEncodedValue( &v );
        return v;
    }

    uint16_t getEncodedValueUInt16() const override
    {
        uint16_t v;
        getEncodedValue( &v );
        return v;
    }

    int32_t getEncodedValueInt32() const override
    {
        int32_t v;
        getEncodedValue( &v );
        return v;
    }

    uint32_t getEncodedValueUInt32() const override
    {
        uint32_t v;
        getEncodedValue( &v );
        return v;
    }

    int64_t getEncodedValueInt64() const override
    {
        int64_t v;
        getEncodedValue( &v );
        return v;
    }

    uint64_t getEncodedValueUInt64() const override
    {
        uint64_t v;
        getEncodedValue( &v );
        return v;
    }

    float getEncodedValueFloat() const override
    {
        float v;
        getEncodedValue( &v );
        return v;
    }

    double getEncodedValueDouble() const override
    {
        double v;
        getEncodedValue( &v );
        return v;
    }

    ///
    /// \brief setFromEncodedValue
    ///
    /// If the encoded_v is out of range, this function will throw range_error
    ///
    /// \param encoded_v The encoded value to use to set the value to
    ///
    /// \return true if the value changed
    template <typename T>
    bool setFromEncodedValue( T encoded_v )
    {
        std::ostringstream buf;
        buf << encoded_v;
        return setValue( buf.str() );
    }

    void setFromEncodedValueAvdeccString( const AvdeccString *storage ) override { m_value = storage->get(); }

    bool setFromEncodedValueInt8( int8_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt8( uint8_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt16( int16_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt16( uint16_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt32( int32_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt32( uint32_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueInt64( int64_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueUInt64( uint64_t v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueFloat( float v ) override { return setFromEncodedValue( v ); }

    bool setFromEncodedValueDouble( double v ) override { return setFromEncodedValue( v ); }

    ///
    /// \brief setFromEncodedValueWithClamp
    ///
    /// If the encoded_v is out of range, this function will clamp the
    /// value to the appropriate range
    ///
    /// \param encoded_v The encoded value to use to set the value to
    ///
    /// \return true if the value changed
    template <typename T>
    bool setFromEncodedValueWithClamp( T encoded_v )
    {
        return setFromEncodedValue( encoded_v );
    }

    bool setFromEncodedValueWithClampInt8( int8_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt8( uint8_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt16( int16_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt16( uint16_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt32( int32_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt32( uint32_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampInt64( int64_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampUInt64( uint64_t v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampFloat( float v ) override { return setFromEncodedValueWithClamp( v ); }

    bool setFromEncodedValueWithClampDouble( double v ) override { return setFromEncodedValueWithClamp( v ); }

    ///
    /// \brief getUnitsCode
    ///
    /// Get the units code for this value
    ///
    /// \return The Units enumeration value
    ///
    UnitsCode getUnitsCode() const override { return UnitsValue; }

    ///
    /// \brief getEncodedMinValue
    /// \return The encoded minimum value
    ///
    int64_t getEncodedMinValue() const override { return min_value; }

    ///
    /// \brief getEncodedMaxValue
    /// \return The encoded maximum value
    ///
    int64_t getEncodedMaxValue() const override { return max_value; }

    ///
    /// \brief getEncodedStepValue
    /// \return The encoded step value
    ///
    int64_t getEncodedStepValue() const override { return step_value; }

    ///
    /// \brief getEncodedDefaultValue
    /// \return the encoded default value
    ///
    int64_t getEncodedDefaultValue() const override { return default_value; }

    ///
    /// \brief getEncodingMultiplierPower
    /// \return the power of 10 used for encoding
    ///
    int8_t getEncodingMultiplierPower() const override { return multiplier_power; }

    const char *getUnitsSuffix() const override { return getAvdeccUnitsSuffix( getUnitsCode() ); }

  private:
    ///
    /// \brief m_value
    ///
    /// The actual non-encoded value
    ///
    value_type m_value;
};

class RangedValueEUI64 : public RangedValueBase
{
    uint64_t m_value;

  public:
    RangedValueEUI64( uint64_t v = 0 ) : m_value( v ) {}
    virtual ~RangedValueEUI64() {}

    bool isReadOnly() const override { return false; }

    ///
    /// \brief getUnitsCode
    ///
    /// \return The UnitsCode for this value
    ///
    UnitsCode getUnitsCode() const override { return UnitsCode::Unitless; }

    ///
    /// \brief getStorageType
    /// \return The EncodingType used for the storage of this value
    ///
    EncodingType getStorageType() const override { return EncodingType::ENCODING_UINT64; }

    ///
    /// \brief getEncodingType
    /// \return The EncodingType used for the encoded transport of this value
    ///
    EncodingType getEncodingType() const override { return EncodingType::ENCODING_UINT64; }
    bool setValue( uint64_t v, bool force = false )
    {
        bool r = false;
        if ( m_value != v )
        {
            m_value = v;
            r = true;
        }
        return r;
    }

    ///
    /// \brief setUnencodedValueString
    ///
    /// If storage type and encoding type are both string, then set
    /// the string value
    ///
    /// \param v The new string value
    /// \return true if the value changed
    ///
    bool setUnencodedValueString( string const &sv, bool force = false ) override
    {
        std::istringstream is( sv );
        is >> std::hex;
        uint64_t v = 0;
        is >> v;
        return setValue( v, force );
    }

    ///
    /// \brief setUnencodedValueBool
    ///
    /// Set the unencoded value from a bool
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueBool( bool v, bool force = false ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief setUnencodedValueFloat
    ///
    /// Set the unencoded value from a float
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueFloat( float v, bool force = false ) override
    {
        throw std::runtime_error( "invalid type for EUI64" );
    }

    ///
    /// \brief setUnencodedValueDouble
    ///
    /// Set the unencoded value from a double
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueDouble( double v, bool force = false ) override
    {
        throw std::runtime_error( "invalid type for EUI64" );
    }

    ///
    /// \brief setUnencodedValueInt64
    ///
    /// Set the unencoded value from an int64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueInt64( int64_t v, bool force = false ) override
    {
        throw std::runtime_error( "invalid type for EUI64" );
    }

    ///
    /// \brief setUnencodedValueUInt64
    ///
    /// Set the unencoded value from an uint64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueUInt64( uint64_t v, bool force = false ) override { return setValue( v ); }

    ///
    /// \brief getUnencodedValueString
    ///
    /// Get the unencoded value string. If the
    /// storage type and encoding type is string, then
    /// it returns the value. If the storage type is not
    /// string then it returns the textual representation
    /// optionally with units
    ///
    /// \return the string representation of the unencoded value
    ///
    string getUnencodedValueString( bool enable_units = true ) const override
    {
        std::ostringstream ss;
        ss << std::hex;
        ss << m_value;
        return ss.str();
    }

    string getUnencodedMinimumString( bool enable_units = true ) const override { return "0"; }

    string getUnencodedMaximumString( bool enable_units = true ) const override { return "ffffffffffffffff"; }

    string getUnencodedDefaultString( bool enable_units = true ) const override { return "0"; }

    string getUnencodedStepString( bool enable_units = true ) const override { return "0"; }

    ///
    /// \brief getUnencodedValueBool
    /// \return the unencoded value as a bool
    ///
    bool getUnencodedValueBool() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief getUnencodedValueFloat
    /// \return the unencoded value as a float
    ///
    float getUnencodedValueFloat() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief getUnencodedValueDouble
    /// \return the unencoded value as a double
    ///
    double getUnencodedValueDouble() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief getUnencodedValueInt64
    /// \return the unencoded value as an int64_t
    ///
    int64_t getUnencodedValueInt64() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief getUnencodedValueUInt64
    /// \return the unencoded value as a uint64_t
    ///
    uint64_t getUnencodedValueUInt64() const override { return m_value; }

    float getUnencodedMinimumFloat() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint64_t getUnencodedMinimumUInt64() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    float getUnencodedMaximumFloat() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint64_t getUnencodedMaximumUInt64() const override { return 0xffffffffffffffffULL; }

    ///
    /// \brief incValue
    ///
    /// Increment the value by the step value
    ///
    /// \return true if the value changed
    ///
    bool incValue() override { return false; }

    ///
    /// \brief decValue
    ///
    /// Decrement the value by the step value
    ///
    /// \return true if the value changed
    ///
    bool decValue() override { return false; }

    void getEncodedValueAvdeccString( AvdeccString *storage ) const override { storage->set( getUnencodedValueString() ); }

    int8_t getEncodedValueInt8() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint8_t getEncodedValueUInt8() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    int16_t getEncodedValueInt16() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint16_t getEncodedValueUInt16() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    int32_t getEncodedValueInt32() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint32_t getEncodedValueUInt32() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    int64_t getEncodedValueInt64() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    uint64_t getEncodedValueUInt64() const override { return m_value; }

    float getEncodedValueFloat() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    double getEncodedValueDouble() const override { throw std::runtime_error( "invalid type for EUI64" ); }

    void setFromEncodedValueAvdeccString( const AvdeccString *storage ) { setUnencodedValueString( storage->get() ); }

    bool setFromEncodedValueInt8( int8_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueUInt8( uint8_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueInt16( int16_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueUInt16( uint16_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueInt32( int32_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueUInt32( uint32_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueInt64( int64_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueUInt64( uint64_t v ) override
    {
        bool changed = false;
        if ( m_value != v )
        {
            m_value = v;
            changed = true;
        }
        return changed;
    }

    bool setFromEncodedValueFloat( float v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueDouble( double v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampInt8( int8_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampUInt8( uint8_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampInt16( int16_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampUInt16( uint16_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampInt32( int32_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampUInt32( uint32_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampInt64( int64_t v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampUInt64( uint64_t v ) override { return setFromEncodedValueUInt64( v ); }

    bool setFromEncodedValueWithClampFloat( float v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    bool setFromEncodedValueWithClampDouble( double v ) override { throw std::runtime_error( "invalid type for EUI64" ); }

    ///
    /// \brief getEncodedMinValue
    /// \return The encoded minimum value
    ///
    int64_t getEncodedMinValue() const override { return 0; }

    ///
    /// \brief getEncodedMaxValue
    /// \return The encoded maximum value
    ///
    int64_t getEncodedMaxValue() const override { return 0; }

    ///
    /// \brief getEncodedStepValue
    /// \return The encoded step value
    ///
    int64_t getEncodedStepValue() const override { return 0; }

    ///
    /// \brief getEncodedDefaultValue
    /// \return the encoded default value
    ///
    int64_t getEncodedDefaultValue() const override { return 0; }

    ///
    /// \brief getEncodingMultiplierPower
    /// \return the power of 10 used for encoding
    ///
    int8_t getEncodingMultiplierPower() const override { return 0; }

    ///
    /// \brief getUnitsSuffix
    ///
    /// Get the units suffix
    ///
    /// \return C string containing UTF8 suffix for the SI Units used
    ///
    const char *getUnitsSuffix() const override { return ""; }

    bool setUnencodedValue( bool v ) { return setUnencodedValueBool( v ); }

    bool setUnencodedValue( double v ) { return setUnencodedValueDouble( v ); }

    bool setUnencodedValue( float v ) { return setUnencodedValueFloat( v ); }

    bool setUnencodedValue( int8_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint8_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int16_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint16_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int32_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint32_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int64_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint64_t v ) { return setUnencodedValueUInt64( v ); }

    void getUnencodedValue( bool *v ) const { *v = getUnencodedValueBool(); }

    void getUnencodedValue( double *v ) const { *v = getUnencodedValueDouble(); }

    void getUnencodedValue( float *v ) const { *v = getUnencodedValueFloat(); }

    void getUnencodedValue( int64_t *v ) const { *v = getUnencodedValueInt64(); }

    void getUnencodedValue( string *v ) const { *v = getUnencodedValueString( false ); }

    void getUnencodedValue( uint64_t *v ) const { *v = getUnencodedValueUInt64(); }
};

template <bool DefaultValue = false, bool MinValue = false, bool MaxValue = true>
class RangedValueBool : public RangedValueBase
{
    bool m_value;

  public:
    RangedValueBool( bool v = DefaultValue ) : m_value( v ) {}
    virtual ~RangedValueBool() {}

    bool isReadOnly() const override { return false; }

    ///
    /// \brief getUnitsCode
    ///
    /// \return The UnitsCode for this value
    ///
    UnitsCode getUnitsCode() const override { return UnitsCode::Unitless; }

    ///
    /// \brief getStorageType
    /// \return The EncodingType used for the storage of this value
    ///
    EncodingType getStorageType() const override { return EncodingType::ENCODING_UINT8; }

    ///
    /// \brief getEncodingType
    /// \return The EncodingType used for the encoded transport of this value
    ///
    EncodingType getEncodingType() const override { return EncodingType::ENCODING_UINT8; }

    bool setValue( bool v, bool force = false )
    {
        bool r = false;
        if ( m_value != v )
        {
            if ( v == MinValue || v == MaxValue || force == true )
            {
                m_value = v;
                r = true;
            }
        }
        return r;
    }

    bool setValue( uint8_t v, bool force = false ) { return setValue( v == 0xff ); }

    ///
    /// \brief setUnencodedValueString
    ///
    /// If storage type and encoding type are both string, then set
    /// the string value
    ///
    /// \param v The new string value
    /// \return true if the value changed
    ///
    bool setUnencodedValueString( string const &sv, bool force = false ) override
    {
        std::istringstream is( sv );
        is >> std::boolalpha;
        bool v = 0;
        is >> v;
        return setValue( v, force );
    }

    ///
    /// \brief setUnencodedValueBool
    ///
    /// Set the unencoded value from a bool
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueBool( bool v, bool force = false ) override { return setValue( v, force ); }

    ///
    /// \brief setUnencodedValueFloat
    ///
    /// Set the unencoded value from a float
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueFloat( float v, bool force = false ) override { return setValue( v != 0.0f, force ); }

    ///
    /// \brief setUnencodedValueDouble
    ///
    /// Set the unencoded value from a double
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueDouble( double v, bool force = false ) override { return setValue( v != 0.0, force ); }

    ///
    /// \brief setUnencodedValueInt64
    ///
    /// Set the unencoded value from an int64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueInt64( int64_t v, bool force = false ) override { return setValue( v != 0, force ); }

    ///
    /// \brief setUnencodedValueUInt64
    ///
    /// Set the unencoded value from an uint64_t
    ///
    /// \param v value to set
    /// \return true if the value changed
    ///
    bool setUnencodedValueUInt64( uint64_t v, bool force = false ) override { return setValue( v != 0, force ); }

    ///
    /// \brief getUnencodedValueString
    ///
    /// Get the unencoded value string. If the
    /// storage type and encoding type is string, then
    /// it returns the value. If the storage type is not
    /// string then it returns the textual representation
    /// optionally with units
    ///
    /// \return the string representation of the unencoded value
    ///
    string getUnencodedValueString( bool enable_units = true ) const override
    {
        std::ostringstream ss;
        ss << std::boolalpha;
        ss << m_value;
        return ss.str();
    }

    string getUnencodedMinimumString( bool enable_units = true ) const override { return MaxValue == true ? "true" : "false"; }

    string getUnencodedMaximumString( bool enable_units = true ) const override { return MinValue == true ? "true" : "false"; }

    string getUnencodedDefaultString( bool enable_units = true ) const override
    {
        return DefaultValue == true ? "true" : "false";
    }

    string getUnencodedStepString( bool enable_units = true ) const override { return "255"; }

    ///
    /// \brief getUnencodedValueBool
    /// \return the unencoded value as a bool
    ///
    bool getUnencodedValueBool() const override { return m_value; }

    ///
    /// \brief getUnencodedValueFloat
    /// \return the unencoded value as a float
    ///
    float getUnencodedValueFloat() const override { return m_value == true ? 1.0f : 0.0f; }

    ///
    /// \brief getUnencodedValueDouble
    /// \return the unencoded value as a double
    ///
    double getUnencodedValueDouble() const override { return m_value == true ? 1.0 : 0.0; }

    ///
    /// \brief getUnencodedValueInt64
    /// \return the unencoded value as an int64_t
    ///
    int64_t getUnencodedValueInt64() const override { return m_value == true ? 1 : 0; }

    ///
    /// \brief getUnencodedValueUInt64
    /// \return the unencoded value as a uint64_t
    ///
    uint64_t getUnencodedValueUInt64() const override { return m_value == true ? 1 : 0; }

    float getUnencodedMinimumFloat() const override { return MinValue == true ? 1.0f : 0.0f; }

    uint64_t getUnencodedMinimumUInt64() const override { return MinValue == true ? 1 : 0; }

    float getUnencodedMaximumFloat() const override { return MaxValue == true ? 1.0f : 0.0f; }

    uint64_t getUnencodedMaximumUInt64() const override { return MaxValue == true ? 1 : 0; }

    ///
    /// \brief incValue
    ///
    /// Increment the value by the step value
    ///
    /// \return true if the value changed
    ///
    bool incValue() override { return setValue( true ); }

    ///
    /// \brief decValue
    ///
    /// Decrement the value by the step value
    ///
    /// \return true if the value changed
    ///
    bool decValue() override { return setValue( false ); }

    void getEncodedValueAvdeccString( AvdeccString *storage ) const override { storage->set( getUnencodedValueString() ); }

    int8_t getEncodedValueInt8() const override { return m_value == true ? -1 : 0; }

    uint8_t getEncodedValueUInt8() const override { return m_value == true ? 0xff : 0; }

    int16_t getEncodedValueInt16() const override { return m_value == true ? 0xff : 0; }

    uint16_t getEncodedValueUInt16() const override { return m_value == true ? 0xff : 0; }

    int32_t getEncodedValueInt32() const override { return m_value == true ? 0xff : 0; }

    uint32_t getEncodedValueUInt32() const override { return m_value == true ? 0xff : 0; }

    int64_t getEncodedValueInt64() const override { return m_value == true ? 0xff : 0; }

    uint64_t getEncodedValueUInt64() const override { return m_value == true ? 0xff : 0; }

    float getEncodedValueFloat() const override { return m_value == true ? 255.0f : 0.0f; }

    double getEncodedValueDouble() const override { return m_value == true ? 255.0 : 0; }

    void setFromEncodedValueAvdeccString( const AvdeccString *storage ) { setUnencodedValueString( storage->get() ); }

    bool setFromEncodedValueInt8( int8_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueUInt8( uint8_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueInt16( int16_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueUInt16( uint16_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueInt32( int32_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueUInt32( uint32_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueInt64( int64_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueUInt64( uint64_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueFloat( float v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueDouble( double v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampInt8( int8_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampUInt8( uint8_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampInt16( int16_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampUInt16( uint16_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampInt32( int32_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampUInt32( uint32_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampInt64( int64_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampUInt64( uint64_t v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampFloat( float v ) override { return setValue( v == 0 ? false : true ); }

    bool setFromEncodedValueWithClampDouble( double v ) override { return setValue( v == 0 ? false : true ); }

    ///
    /// \brief getEncodedMinValue
    /// \return The encoded minimum value
    ///
    int64_t getEncodedMinValue() const override { return MinValue == true ? 0xff : 0; }

    ///
    /// \brief getEncodedMaxValue
    /// \return The encoded maximum value
    ///
    int64_t getEncodedMaxValue() const override { return MaxValue == true ? 0xff : 0; }

    ///
    /// \brief getEncodedStepValue
    /// \return The encoded step value
    ///
    int64_t getEncodedStepValue() const override { return 0xff; }

    ///
    /// \brief getEncodedDefaultValue
    /// \return the encoded default value
    ///
    int64_t getEncodedDefaultValue() const override { return DefaultValue == true ? 0xff : 0; }

    ///
    /// \brief getEncodingMultiplierPower
    /// \return the power of 10 used for encoding
    ///
    int8_t getEncodingMultiplierPower() const override { return 0; }

    ///
    /// \brief getUnitsSuffix
    ///
    /// Get the units suffix
    ///
    /// \return C string containing UTF8 suffix for the SI Units used
    ///
    const char *getUnitsSuffix() const override { return ""; }

    bool setUnencodedValue( bool v ) { return setUnencodedValueBool( v ); }

    bool setUnencodedValue( double v ) { return setUnencodedValueDouble( v ); }

    bool setUnencodedValue( float v ) { return setUnencodedValueFloat( v ); }

    bool setUnencodedValue( int8_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint8_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int16_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint16_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int32_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint32_t v ) { return setUnencodedValueUInt64( v ); }

    bool setUnencodedValue( int64_t v ) { return setUnencodedValueInt64( v ); }

    bool setUnencodedValue( uint64_t v ) { return setUnencodedValueUInt64( v ); }

    void getUnencodedValue( bool *v ) const { *v = getUnencodedValueBool(); }

    void getUnencodedValue( double *v ) const { *v = getUnencodedValueDouble(); }

    void getUnencodedValue( float *v ) const { *v = getUnencodedValueFloat(); }

    void getUnencodedValue( int64_t *v ) const { *v = getUnencodedValueInt64(); }

    void getUnencodedValue( string *v ) const { *v = getUnencodedValueString( false ); }

    void getUnencodedValue( uint64_t *v ) const { *v = getUnencodedValueUInt64(); }
};
}
