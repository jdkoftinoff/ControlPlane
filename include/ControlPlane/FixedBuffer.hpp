#pragma once

#include "World.hpp"
#include "Eui.hpp"

namespace ControlPlane
{

///
/// \brief The FixedBuffer class
///
/// Manages the appending, setting, and getting of network order
/// values in an arbitrary byte buffer
///
class FixedBuffer
{
  protected:
    uint8_t *m_buf;
    uint16_t m_length;
    uint16_t m_max_length;

  public:
    ///
    /// \brief FixedBuffer constructor, hold pointer to buffer and maximum
    /// length
    /// \param buf pointer to buffer
    /// \param max_length length of buffer
    ///
    FixedBuffer( uint8_t *buf, uint16_t max_length ) : m_buf( buf ), m_length( 0 ), m_max_length( max_length ) {}

    ///
    /// \brief isFull test if the buffer is full or would be full with
    /// additional data
    /// \param additional additional octet count
    /// \return true if the buffer is or would be full
    ///
    bool isFull( uint16_t additional = 0 ) const { return ( m_length + additional ) >= m_max_length; }

    ///
    /// \brief canPut test if the buffer can accept more octets
    /// \param additional octet count
    /// \return true if there is enough buffer space
    ///
    bool canPut( uint16_t additional = 1 ) const { return ( m_length + additional ) <= m_max_length; }

    ///
    /// \brief getMaxLength get maximum buffer length
    /// \return the maximum buffer length
    ///
    uint16_t getMaxLength() const { return m_max_length; }

    ///
    /// \brief putEUI48 append a Eui48 to the buffer
    /// \param val the Eui48
    ///
    void putEUI48( Eui48 const &val )
    {
        for ( uint16_t i = 0; i < 6; ++i )
        {
            m_buf[m_length++] = val.value[i];
        }
    }

    ///
    /// \brief putEUI48 append a zeroed out Eui48 to the buffer
    ///
    void putEUI48()
    {
        for ( uint16_t i = 0; i < 6; ++i )
        {
            m_buf[m_length++] = 0;
        }
    }

    ///
    /// \brief putEUI64 append a zeroed out Eui64 to the buffer
    ///
    void putEUI64()
    {
        for ( uint16_t i = 0; i < 8; ++i )
        {
            m_buf[m_length++] = 0;
        }
    }

    ///
    /// \brief putEUI64 append a Eui64 to the buffer
    /// \param val the Eui64
    ///
    void putEUI64( Eui64 const &val )
    {
        for ( uint16_t i = 0; i < 8; ++i )
        {
            m_buf[m_length++] = val.value[i];
        }
    }

    ///
    /// \brief putAvdeccString append an AVDECC style string to the buffer
    /// \param val reference to std::string
    ///
    void putAvdeccString( std::string const &val ) { putAvdeccString( val.c_str() ); }

    ///
    /// \brief putAvdeccString append a c string as AVDECC String
    /// Convert the C string to an AVDECC style string and append it to the
    /// buffer
    /// \param val pointer to c string
    ///
    void putAvdeccString( const char *val )
    {
        bool ended = false;
        for ( uint16_t i = 0; i < 64; ++i )
        {
            if ( ended )
            {
                m_buf[m_length++] = 0;
            }
            else
            {
                m_buf[m_length++] = val[i];
                if ( val[i] == 0 )
                {
                    ended = true;
                }
            }
        }
    }

    ///
    /// \brief putAvdeccString append an empty AVDECC String
    ///
    void putAvdeccString() { putZeros( 64 ); }

    ///
    /// \brief putOctet append a single Octet (8 bit number) to the buffer
    /// \param val the Octet to append
    ///
    void putOctet( uint8_t val ) { m_buf[m_length++] = val; }

    ///
    /// \brief putDoublet append a single Doublet (16 bit number) to the buffer
    /// \param val the Doublet to append
    ///
    void putDoublet( uint16_t val )
    {
        m_buf[m_length++] = ( val >> 8 ) & 0xff;
        m_buf[m_length++] = ( val >> 0 ) & 0xff;
    }

    ///
    /// \brief putQuadlet append a single Quadlet (32 bit number) to the buffer
    /// \param val the Quadlet to append
    ///
    void putQuadlet( uint32_t val )
    {
        m_buf[m_length++] = ( val >> 24 ) & 0xff;
        m_buf[m_length++] = ( val >> 16 ) & 0xff;
        m_buf[m_length++] = ( val >> 8 ) & 0xff;
        m_buf[m_length++] = ( val >> 0 ) & 0xff;
    }

    ///
    /// \brief putOctlet append a single Octlet (64 bit number) to the buffer
    /// \param val the Octlet to append
    ///
    void putOctlet( uint64_t val )
    {
        m_buf[m_length++] = ( val >> 56 ) & 0xff;
        m_buf[m_length++] = ( val >> 48 ) & 0xff;
        m_buf[m_length++] = ( val >> 40 ) & 0xff;
        m_buf[m_length++] = ( val >> 32 ) & 0xff;
        m_buf[m_length++] = ( val >> 24 ) & 0xff;
        m_buf[m_length++] = ( val >> 16 ) & 0xff;
        m_buf[m_length++] = ( val >> 8 ) & 0xff;
        m_buf[m_length++] = ( val >> 0 ) & 0xff;
    }

    ///
    /// \brief putZeros append a number Octets with value 0 to the buffer
    /// \param count the count of Octets to append
    ///
    void putZeros( uint16_t count )
    {
        for ( uint16_t i = 0; i < count; ++i )
        {
            m_buf[m_length++] = 0;
        }
    }

    ///
    /// \brief putBuf append an arbitrary number of bytes to the buffer
    /// \param buf The buffer to read from
    /// \param len The count of bytes
    ///
    void putBuf( const uint8_t *buf, uint16_t len )
    {
        for ( uint16_t i = 0; i < len; ++i )
        {
            m_buf[m_length++] = buf[i];
        }
    }

    ///
    /// \brief putBuf append a FixedBuffer to the buffer
    /// \param buf the FixedBuffer to copy
    ///
    void putBuf( FixedBuffer const &buf )
    {
        for ( uint16_t i = 0; i < buf.getLength(); ++i )
        {
            m_buf[m_length++] = buf.getOctet( i );
        }
    }

    ///
    /// \brief setOctet Set an Octet in the buffer
    /// \param val Octet value
    /// \param pos Position within buffer
    ///
    void setOctet( uint8_t val, uint16_t pos ) { m_buf[pos] = val; }

    ///
    /// \brief getOctet Get an Octet from the buffer
    /// \param pos Position within buffer
    /// \return the Octet value
    ///
    uint8_t getOctet( uint16_t pos ) const { return m_buf[pos]; }

    ///
    /// \brief setDoublet Set an Doublet in the buffer
    /// \param val Doublet value
    /// \param pos Position within buffer
    ///
    void setDoublet( uint16_t v, uint16_t pos )
    {
        uint8_t *b = &m_buf[pos];
        b[0] = ( uint8_t )( ( v ) >> 8 ) & 0xff;
        b[1] = ( uint8_t )( ( v ) >> 0 ) & 0xff;
    }

    ///
    /// \brief getDoublet Get an Doublet from the buffer
    /// \param pos Position within buffer
    /// \return the Doublet value
    ///
    uint16_t getDoublet( uint16_t pos ) const
    {
        uint8_t const *b = &m_buf[pos];

        return ( ( (uint16_t)b[0] ) << 8 ) + b[1];
    }

    ///
    /// \brief setQuadlet Set an Quadlet in the buffer
    /// \param val Quadlet value
    /// \param pos Position within buffer
    ///
    void setQuadlet( uint32_t v, uint16_t pos )
    {
        uint8_t *b = &m_buf[pos];
        b[0] = ( uint8_t )( ( v ) >> 24 ) & 0xff;
        b[1] = ( uint8_t )( ( v ) >> 16 ) & 0xff;
        b[2] = ( uint8_t )( ( v ) >> 8 ) & 0xff;
        b[3] = ( uint8_t )( ( v ) >> 0 ) & 0xff;
    }

    ///
    /// \brief getQuadlet Get an Quadlet from the buffer
    /// \param pos Position within buffer
    /// \return the Quadlet value
    ///
    uint32_t getQuadlet( uint16_t pos ) const
    {
        uint8_t const *b = &m_buf[pos];

        return ( ( (uint32_t)b[0] ) << 24 ) + ( ( (uint32_t)b[1] ) << 16 ) + ( ( (uint32_t)b[2] ) << 8 ) + b[3];
    }

    ///
    /// \brief setEUI48 set an EUI48 value in the buffer
    /// \param v Eui48 value
    /// \param pos Position within buffer
    ///
    void setEUI48( Eui48 v, uint16_t pos ) { v.store( m_buf, pos ); }

    ///
    /// \brief getEUI48 Get an Eui48 from the buffer
    /// \param pos Position within buffer
    /// \return Eui48 value
    ///
    Eui48 getEUI48( uint16_t pos ) const { return Eui48( m_buf + pos ); }

    ///
    /// \brief getEui64 Get an Eui64 from the buffer
    /// \param pos Position within buffer
    /// \return Eui64 value
    ///
    void setEUI64( Eui64 v, uint16_t pos ) { v.store( m_buf, pos ); }

    ///
    /// \brief getEui64 Get an Eui64 from the buffer
    /// \param pos Position within buffer
    /// \return Eui64 value
    ///
    Eui64 getEUI64( uint16_t pos ) const { return Eui64( m_buf + pos ); }

    ///
    /// \brief getBuf Get the raw pointer to the buffer
    /// \return uint8_t const Buffer pointer
    ///
    uint8_t const *getBuf() const { return m_buf; }

    ///
    /// \brief getBuf Get the raw pointer to the buffer
    /// \return uint8_t Buffer pointer
    ///
    uint8_t *getBuf() { return m_buf; }

    ///
    /// \brief getBuf Get the raw pointer to the buffer at an offset
    /// \param pos Position within buffer
    /// \return uint8_t const buffer pointer
    ///
    uint8_t const *getBuf( uint16_t pos ) const { return &m_buf[pos]; }

    ///
    /// \brief getBuf Get the raw pointer to the buffer at an offset
    /// \param pos Position within buffer
    /// \return uint8_t buffer pointer
    ///
    uint8_t *getBuf( uint16_t pos ) { return &m_buf[pos]; }

    ///
    /// \brief getLength Get The actual buffer length in Octets
    /// \return Buffer length in Octets
    ///
    uint16_t getLength() const { return m_length; }

    ///
    /// \brief setLength Set the Actual buffer length in Octets
    /// \param n The new buffer length in Octets
    ///
    void setLength( uint16_t n ) { m_length = n; }

    ///
    /// \brief clear Set the actual buffer length to 0
    ///
    void clear() { m_length = 0; }
};

///
/// \brief The FixedBufferWithSize class
///
/// A subclass of FixedBuffer which is templatized on a MaxSize
/// argument and allocates a static buffer of that size
///
template <size_t MaxSize>
class FixedBufferWithSize : public FixedBuffer
{
  protected:
    ///
    /// \brief m_buf_storage The storage for the buffer
    ///
    uint8_t m_buf_storage[MaxSize];

  public:
    ///
    /// \brief FixedBufferWithSize Default constructor
    /// initializes the buffer pointer and size
    ///
    FixedBufferWithSize() : FixedBuffer( m_buf_storage, MaxSize ) {}
};
}
