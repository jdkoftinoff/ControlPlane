#pragma once

#include "World.hpp"

namespace ControlPlane
{

///
/// \brief The Eui48 class hold an IEEE EUI48 / MAC Address
///
class Eui48
{
  public:
    uint8_t value[6];

    ///
    /// \brief Eui48 constructor
    ///
    /// Initialize all octets to 0xff
    ///
    Eui48() { clear(); }

    ///
    /// \brief Eui48 Construct with 6 octet values
    /// \param a-f The 6 octets that make up the EUI48
    ///
    Eui48( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f )
    {
        value[0] = a;
        value[1] = b;
        value[2] = c;
        value[3] = d;
        value[4] = e;
        value[5] = f;
    }

    ///
    /// \brief Eui48 Construct via pointer to 6 octet values
    /// \param p pointer to 6 octets
    ///
    Eui48( uint8_t const *p ) { memcpy( value, p, sizeof( value ) ); }

    ///
    /// \brief Eui48 construct with right justified uint64_t
    /// \param v uint64_t with low 48 bits containing EUI48
    ///
    Eui48( uint64_t v )
    {
        value[0] = ( uint8_t )( ( v >> ( 5 * 8 ) ) & 0xff );
        value[1] = ( uint8_t )( ( v >> ( 4 * 8 ) ) & 0xff );
        value[2] = ( uint8_t )( ( v >> ( 3 * 8 ) ) & 0xff );
        value[3] = ( uint8_t )( ( v >> ( 2 * 8 ) ) & 0xff );
        value[4] = ( uint8_t )( ( v >> ( 1 * 8 ) ) & 0xff );
        value[5] = ( uint8_t )( ( v >> ( 0 * 8 ) ) & 0xff );
    }

    Eui48 &operator=( Eui48 const &other ) = default;

    ///
    /// \brief convertToUint64 convert to uint64_t
    /// \return uint64_t with lower 48 bits containing EUI48
    ///
    uint64_t convertToUint64() const
    {
        uint64_t v = 0;
        v |= ( (uint64_t)value[0] ) << ( 5 * 8 );
        v |= ( (uint64_t)value[1] ) << ( 4 * 8 );
        v |= ( (uint64_t)value[2] ) << ( 3 * 8 );
        v |= ( (uint64_t)value[3] ) << ( 2 * 8 );
        v |= ( (uint64_t)value[4] ) << ( 1 * 8 );
        v |= ( (uint64_t)value[5] ) << ( 0 * 8 );
        return v;
    }

    ///
    /// \brief store Store octets into buffer
    /// \param p destination buffer pointer
    /// \param pos offset from start of buffer
    ///
    void store( uint8_t *p, size_t pos ) const { memcpy( p + pos, &value[0], sizeof( value ) ); }

    ///
    /// \brief isSet checks if Eui48 has been initialized
    /// \return true if the EUI48 is not FF:FF:FF:FF:FF:FF
    ///
    bool isSet() const
    {
        bool r = false;
        if ( value[0] != 0xff || value[1] != 0xff || value[2] != 0xff || value[3] != 0xff || value[4] != 0xff
             || value[5] != 0xff )
        {
            r = true;
        }
        return r;
    }

    ///
    /// \brief isNotSet checks if Eui48 is set to FF:FF:FF:FF:FF:FF
    /// \return true if EUI48 is FF:FF:FF:FF:FF:FF
    ///
    bool isNotSet() const { return !isSet(); }

    ///
    /// \brief isZero checks if Eui48 is set to 00:00:00:00:00:00
    /// \return true if EUI48 is 00:00:00:00:00:00
    ///
    bool isZero() const
    {
        bool r = false;
        if ( value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 0 && value[4] == 0 && value[5] == 0 )
        {
            r = true;
        }
        return r;
    }

    ///
    /// \brief isNotZero checks if Eui48 is not set to 00:00:00:00:00:00
    /// \return true if EUI48 is not 00:00:00:00:00:00
    ///
    bool isNotZero() const { return !isZero(); }

    void zero()
    {
        value[0] = 0;
        value[1] = 0;
        value[2] = 0;
        value[3] = 0;
        value[4] = 0;
        value[5] = 0;
    }

    ///
    /// \brief compare Numeric compare with other Eui48
    /// \param other Eui48 reference to compare to
    /// \return -1 if less than other, 0 if equal to other, 1 if greater than
    /// other
    ///
    int compare( const Eui48 &other ) const { return memcmp( value, other.value, sizeof( value ) ); }

    void clear()
    {
        value[0] = 0xff;
        value[1] = 0xff;
        value[2] = 0xff;
        value[3] = 0xff;
        value[4] = 0xff;
        value[5] = 0xff;
    }
};

///
/// \brief The Eui64 class hold an IEEE EUI64
///
class Eui64
{
  public:
    uint8_t value[8];

    ///
    /// \brief Eui64 constructor
    ///
    /// Initialize all octets to 0xff
    ///
    Eui64() { clear(); }

    ///
    /// \brief Eui48 Construct with 8 octet values
    /// \param a-h The 8 octets that make up the EUI48
    ///
    Eui64( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h )
    {
        value[0] = a;
        value[1] = b;
        value[2] = c;
        value[3] = d;
        value[4] = e;
        value[5] = f;
        value[6] = g;
        value[7] = h;
    }

    ///
    /// \brief Eui64 Construct via pointer to 8 octet values
    /// \param p pointer to 8 octets
    ///
    Eui64( uint8_t const *p ) { memcpy( value, p, sizeof( value ) ); }

    ///
    /// \brief Eui64 copy constructor from Eui64
    /// \param other
    ///
    Eui64( Eui64 const &other ) = default;

    ///
    /// \brief Eui48 construct with right justified uint64_t
    /// \param v uint64_t containing EUI48
    ///
    Eui64( uint64_t v )
    {
        value[0] = ( uint8_t )( ( v >> ( 7 * 8 ) ) & 0xff );
        value[1] = ( uint8_t )( ( v >> ( 6 * 8 ) ) & 0xff );
        value[2] = ( uint8_t )( ( v >> ( 5 * 8 ) ) & 0xff );
        value[3] = ( uint8_t )( ( v >> ( 4 * 8 ) ) & 0xff );
        value[4] = ( uint8_t )( ( v >> ( 3 * 8 ) ) & 0xff );
        value[5] = ( uint8_t )( ( v >> ( 2 * 8 ) ) & 0xff );
        value[6] = ( uint8_t )( ( v >> ( 1 * 8 ) ) & 0xff );
        value[7] = ( uint8_t )( ( v >> ( 0 * 8 ) ) & 0xff );
    }

    ///
    /// \brief operator = from Eui64
    /// \param other reference to Eui64
    /// \return reference to self
    ///
    Eui64 &operator=( Eui64 const &other ) = default;

    ///
    /// \brief convertToUint64 convert to uint64_t
    /// \return uint64_t containing EUI48
    ///
    uint64_t convertToUint64() const
    {
        uint64_t v = 0;
        v |= ( (uint64_t)value[0] ) << ( 7 * 8 );
        v |= ( (uint64_t)value[1] ) << ( 6 * 8 );
        v |= ( (uint64_t)value[2] ) << ( 5 * 8 );
        v |= ( (uint64_t)value[3] ) << ( 4 * 8 );
        v |= ( (uint64_t)value[4] ) << ( 3 * 8 );
        v |= ( (uint64_t)value[5] ) << ( 2 * 8 );
        v |= ( (uint64_t)value[6] ) << ( 1 * 8 );
        v |= ( (uint64_t)value[7] ) << ( 0 * 8 );
        return v;
    }

    ///
    /// \brief store Store octets into buffer
    /// \param p destination buffer pointer
    /// \param pos offset from start of buffer
    ///
    void store( uint8_t *p, size_t pos ) const { memcpy( p + pos, &value[0], 8 ); }

    ///
    /// \brief isSet checks is Eui64 has been initialized
    /// \return true if the Eui64 is not FF:FF:FF:FF:FF:FF:FF:FF
    ///
    bool isSet() const
    {
        bool r = false;
        if ( value[0] != 0xff || value[1] != 0xff || value[2] != 0xff || value[3] != 0xff || value[4] != 0xff
             || value[5] != 0xff || value[6] != 0xff || value[7] != 0xff )
        {
            r = true;
        }
        return r;
    }

    ///
    /// \brief isSet checks is Eui64 has not been initialized
    /// \return true if the EUI64 is FF:FF:FF:FF:FF:FF:FF:FF
    ///
    bool isUnset() const { return !isSet(); }

    ///
    /// \brief isSet checks is Eui48 is 0
    /// \return true if the EUI64 is 00:00:00:00:00:00:00:00
    ///
    bool isZero() const
    {
        bool r = false;
        if ( value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 0 && value[4] == 0 && value[5] == 0 && value[6] == 0
             && value[7] == 0 )
        {
            r = true;
        }
        return r;
    }

    ///
    /// \brief compare Numeric compare with other Eui64
    /// \param other Eui64 reference to compare to
    /// \return -1 if less than other, 0 if equal to other, 1 if greater than
    /// other
    ///
    int compare( const Eui64 &other ) const { return memcmp( value, other.value, sizeof( value ) ); }

    void clear()
    {
        value[0] = 0xff;
        value[1] = 0xff;
        value[2] = 0xff;
        value[3] = 0xff;
        value[4] = 0xff;
        value[5] = 0xff;
        value[6] = 0xff;
        value[7] = 0xff;
    }
};

inline bool operator<( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) < 0; }

inline bool operator<=( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) <= 0; }

inline bool operator==( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) == 0; }

inline bool operator>=( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) >= 0; }

inline bool operator>( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) > 0; }

inline bool operator!=( const Eui48 &lhs, const Eui48 &rhs ) { return lhs.compare( rhs ) != 0; }

inline bool operator<( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) < 0; }

inline bool operator<=( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) <= 0; }

inline bool operator==( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) == 0; }

inline bool operator>=( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) >= 0; }

inline bool operator>( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) > 0; }

inline bool operator!=( const Eui64 &lhs, const Eui64 &rhs ) { return lhs.compare( rhs ) != 0; }

std::ostream &operator<<( std::ostream &o, Eui48 const &v );

std::ostream &operator<<( std::ostream &o, Eui64 const &v );
}
