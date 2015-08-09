#pragma once

#include "World.hpp"

namespace ControlPlane
{

///
/// \brief The AvdeccString class
///
/// Interface for the Avdecc String. Contains a pointer to the actual
/// raw data storage and length. Does not allocate the storage.
///
class AvdeccString
{
  public:
    AvdeccString( uint8_t *buf, uint16_t buf_size ) : m_buf( buf ), m_buf_size( buf_size ) {}

    virtual void clear() = 0;
    virtual void set( const string &s ) = 0;
    virtual string get() const = 0;

    uint8_t *m_buf;
    uint16_t m_buf_size;
};

template <size_t SZ, bool NullTerminated>
class AvdeccControlStringWithSize : public AvdeccString
{
  public:
    ///
    /// \brief AvdeccControlStringWithSize
    ///
    /// Initialize all characters in the string to null
    ///
    AvdeccControlStringWithSize() : AvdeccString( m_storage, SZ )
    {
        for ( int i = 0; i < sizeof( m_storage ); ++i )
        {
            m_storage[i] = 0;
        }
    }

    ///
    /// \brief AvdeccControlStringWithSize
    ///
    /// Copy constructor
    ///
    /// \param other
    ///
    AvdeccControlStringWithSize( const AvdeccString &other ) : AvdeccString( m_storage, SZ )
    {
        clear();
        uint16_t max_to_copy;
        if ( NullTerminated )
        {
            max_to_copy = sizeof( m_storage ) - 1;
            if ( other.m_buf_size - 1 < max_to_copy )
            {
                max_to_copy = other.m_buf_size - 1;
            }
        }
        else
        {
            max_to_copy = sizeof( m_storage );
            if ( other.m_buf_size < max_to_copy )
            {
                max_to_copy = other.m_buf_size;
            }
        }
        memcpy( m_storage, other.m_buf, max_to_copy );
    }

    ///
    /// \brief operator =
    ///
    /// Assignment operator copies all characters
    ///
    /// \param other
    /// \return *this
    ///
    AvdeccControlStringWithSize &operator=( const AvdeccString &other )
    {
        clear();
        uint16_t max_to_copy;
        if ( NullTerminated )
        {
            max_to_copy = sizeof( m_storage ) - 1;
            if ( other.m_buf_size - 1 < max_to_copy )
            {
                max_to_copy = other.m_buf_size - 1;
            }
        }
        else
        {
            max_to_copy = sizeof( m_storage );
            if ( other.m_buf_size < max_to_copy )
            {
                max_to_copy = other.m_buf_size;
            }
        }
        memcpy( m_storage, other.m_buf, max_to_copy );
        return *this;
    }

    ///
    /// \brief AvdeccControlString
    ///
    /// Constructor which takes a std::string
    ///
    /// \param s
    ///
    AvdeccControlStringWithSize( const string &s ) : AvdeccString( m_storage, SZ ) { set( s ); }

    ///
    /// \brief operator =
    ///
    /// Assignment operator copies all characters from string
    ///
    /// \param other
    /// \return *this
    ///
    AvdeccControlStringWithSize &operator=( const string &other )
    {
        set( other );
        return *this;
    }

    ///
    /// \brief clear
    ///
    /// Initialize all characters in the string to null
    ///
    void clear() override
    {
        for ( size_t i = 0; i < sizeof( m_storage ); ++i )
        {
            m_storage[i] = 0;
        }
    }

    ///
    /// \brief set
    ///
    /// Set the string from a std::string, making sure it is null terminated
    ///
    /// \param s string
    ///
    void set( const string &s ) override
    {
        clear();
        size_t count = sizeof( m_storage );
        if ( NullTerminated )
        {
            count -= 1;
        }
        for ( size_t i = 0; i < s.length() && i < count; ++i )
        {
            if ( s[i] != 0 )
            {
                m_storage[i] = uint8_t( s[i] );
            }
        }
    }

    ///
    /// \brief get
    ///
    /// Get the value as a std::string
    ///
    /// \return string
    ///
    string get() const override
    {
        string s;
        size_t count = sizeof( m_storage );
        if ( NullTerminated )
        {
            count -= 1;
        }
        for ( size_t i = 0; i < count; ++i )
        {
            if ( m_storage[i] == 0 )
            {
                break;
            }
            s.push_back( char( m_storage[i] ) );
        }
        return s;
    }

  private:
    /// Null terminated UTF-8 string, 406 octets long max including null termination
    uint8_t m_storage[SZ];
};

///
/// \brief The String used for GET_NAME/SET_NAME and object_name fields is 64 octets long without requiring
/// null termination
///
///
using AvdeccNameString = AvdeccControlStringWithSize<64, false>;

///
/// \brief The string used for string values transported via GET_CONTROL/SET_CONTROL can be up to 406 octets
/// long and are required to be null terminated
///
using AvdeccControlString = AvdeccControlStringWithSize<406, true>;
}
