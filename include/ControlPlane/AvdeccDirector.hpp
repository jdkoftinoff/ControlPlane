#pragma once

#include "World.hpp"
#include "Avdecc.hpp"
#include "Util.hpp"
#include "Eui.hpp"
#include "FixedBuffer.hpp"

namespace ControlPlane
{

template <typename AddressType>
struct ACMPMulticast
{
    using address_type = AddressType;
    static AddressType getDestination();
};

template <>
inline Eui48 ACMPMulticast<Eui48>::getDestination()
{
    return Eui48( 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 );
}

template <>
inline sockaddr_in ACMPMulticast<sockaddr_in>::getDestination()
{
    sockaddr_in r;

    r.sin_family = AF_INET;
    r.sin_len = sizeof( sockaddr_in );
    r.sin_port = htons( 17221 );
    r.sin_addr.s_addr = htonl( 0xe00000fbL );
    return r;
}

template <>
inline sockaddr_in6 ACMPMulticast<sockaddr_in6>::getDestination()
{
    sockaddr_in6 r;

    r.sin6_family = AF_INET6;
    r.sin6_len = sizeof( sockaddr_in6 );
    r.sin6_port = htons( 17221 );
    inet_pton( AF_INET6, "ff02::fb", &r.sin6_addr );

    return r;
}

template <typename AddressType>
using ADPMulticast = ACMPMulticast<AddressType>;

template <typename AddressType>
struct AECPMulticast
{
    using address_type = AddressType;
    static AddressType getDestination();
};

template <>
inline Eui48 AECPMulticast<Eui48>::getDestination()
{
    return Eui48( 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x01 );
}

template <>
inline sockaddr_in AECPMulticast<sockaddr_in>::getDestination()
{
    sockaddr_in r;

    r.sin_family = AF_INET;
    r.sin_len = sizeof( sockaddr_in );
    r.sin_port = htons( 17221 );
    r.sin_addr.s_addr = htonl( 0xe00000fbL );
    return r;
}

template <>
inline sockaddr_in6 AECPMulticast<sockaddr_in6>::getDestination()
{
    sockaddr_in6 r;

    r.sin6_family = AF_INET6;
    r.sin6_len = sizeof( sockaddr_in6 );
    r.sin6_port = htons( 17221 );
    inet_pton( AF_INET6, "ff02::fb", &r.sin6_addr );

    return r;
}

template <typename AddressType>
struct MAAPMulticast
{
    using address_type = AddressType;
    static AddressType getDestination();
};

template <>
inline Eui48 MAAPMulticast<Eui48>::getDestination()
{
    return Eui48( 0x91, 0xe0, 0xf0, 0x00, 0xff, 0x00 );
}

template <>
inline sockaddr_in MAAPMulticast<sockaddr_in>::getDestination()
{
    sockaddr_in r;

    r.sin_family = AF_INET;
    r.sin_len = sizeof( sockaddr_in );
    r.sin_port = htons( 17221 );
    r.sin_addr.s_addr = htonl( 0xe00000fbL );
    return r;
}

template <>
inline sockaddr_in6 MAAPMulticast<sockaddr_in6>::getDestination()
{
    sockaddr_in6 r;

    r.sin6_family = AF_INET6;
    r.sin6_len = sizeof( sockaddr_in6 );
    r.sin6_port = htons( 17221 );
    inet_pton( AF_INET6, "ff02::fb", &r.sin6_addr );

    return r;
}

template <typename AddressType>
class AVDECCDirector
{
    struct EntityAddressInfo
    {
        Milliseconds m_last_seen_time;
        Eui64 m_entity_id;
        AddressType m_remote_addr;
    };

    void flushKnownAddressesFromMap( std::map<Eui64, EntityAddressInfo> &m, Milliseconds threshold, Milliseconds current_time )
    {
        for ( auto it = m.begin(); it != m.end(); )
        {
            if ( it->second.m_last_seen_time + threshold < current_time )
            {
                m.erase( it++ );
            }
            else
            {
                it++;
            }
        }
    }

  public:
    AVDECCDirector( Milliseconds expiry_time_in_milliseconds = Milliseconds( 60000 ),
                    Milliseconds flush_period_in_miliseconds = Milliseconds( 5000 ) )
        : m_expiry_time_in_milliseconds( expiry_time_in_milliseconds )
        , m_flush_period_in_milliseconds( flush_period_in_miliseconds )
        , m_last_flush_time( Milliseconds( 0 ) )
    {
    }

    void tick( Milliseconds current_time_in_milliseconds )
    {
        if ( m_last_flush_time == Milliseconds( 0 ) )
        {
            // initialize last_flush_time
            m_last_flush_time = current_time_in_milliseconds;
        }
        else
        {
            // is it time to check?
            if ( ( current_time_in_milliseconds - m_last_flush_time ) > m_flush_period_in_milliseconds )
            {
                // yes, remember when we did
                m_last_flush_time = current_time_in_milliseconds;

                // flush any expired addresses
                flushKnownAddressesFromMap(
                    m_known_entity_addresses, m_expiry_time_in_milliseconds, current_time_in_milliseconds );
            }
        }
    }

    void seenReceivedPayload( Milliseconds time_in_milliseconds, FixedBuffer const &payload, AddressType const &remote_addr )
    {
        Eui64 entity_id;
        if ( getRemoteEntityIdFromReceivedAVTPDU( &entity_id, payload ) )
        {
            seenEntityId( time_in_milliseconds, entity_id, remote_addr );
        }
    }

    void seenEntityId( Milliseconds time_in_milliseconds, Eui64 entity_id, AddressType const &remote_addr )
    {
        m_known_entity_addresses[entity_id] = EntityAddressInfo{time_in_milliseconds, entity_id, remote_addr};
    }

    bool isMessageMulticast( AddressType *addr, FixedBuffer const &payload, bool multicast = false ) const
    {
        bool r = false;

        if ( payload.getLength() > 12 )
        {
            uint8_t subtype = payload.getOctet( 0 );
            uint8_t sv_version_control_data = payload.getOctet( 1 );

            if ( ( sv_version_control_data & 0xf0 ) == 0 )
            {
                if ( subtype == 0x7a )
                {
                    // ADP is always multicast
                    r = true;
                    *addr = ADPMulticast<AddressType>::getDestination();
                }
                else if ( subtype == 0x7c )
                {
                    // ACMP is always multicast
                    r = true;
                    *addr = ACMPMulticast<AddressType>::getDestination();
                }
                else if ( subtype == 0x7b )
                {
                    // AECP is only multicast for IDENTIFY
                    r = multicast;
                    if ( r )
                    {
                        *addr = AECPMulticast<AddressType>::getDestination();
                    }
                }
                else if ( subtype == 0x7e )
                {
                    // MAAP is always multicast
                    r = true;
                    *addr = MAAPMulticast<AddressType>::getDestination();
                }
            }
        }

        return r;
    }

    bool getRemoteEntityIdFromReceivedAVTPDU( Eui64 *entity_id, FixedBuffer const &payload ) const
    {
        bool r = false;
        uint8_t subtype = payload.getOctet( 0 );
        uint8_t sv_version_control_data = payload.getOctet( 1 );

        // AECP only
        if ( subtype == 0x7b )
        {
            if ( ( sv_version_control_data & 0xf0 ) == 0 )
            {
                uint8_t command_type = sv_version_control_data & 0xf;
                bool is_command = ( command_type & 1 ) == 0;

                if ( is_command )
                {
                    // entity_id is controller_entity_id
                    *entity_id = payload.getEUI64( 12 );
                    r = true;
                }
                else
                {
                    // entity_id is target_entity_id
                    *entity_id = payload.getEUI64( 4 );
                    r = true;
                }
            }
        }
        return r;
    }

    bool getRemoteEntityIdFromSentAVTPDU( Eui64 *entity_id, FixedBuffer const &payload ) const
    {
        bool r = false;
        uint8_t subtype = payload.getOctet( 0 );
        uint8_t sv_version_control_data = payload.getOctet( 1 );

        // AECP only
        if ( subtype == 0x7b )
        {
            if ( ( sv_version_control_data & 0xf0 ) == 0 )
            {
                uint8_t command_type = sv_version_control_data & 0xf;
                bool is_command = ( command_type & 1 ) == 0;

                if ( is_command )
                {
                    // entity_id is target_entity_id
                    *entity_id = payload.getEUI64( 4 );
                    r = true;
                }
                else
                {
                    // entity_id is controller_entity_id
                    *entity_id = payload.getEUI64( 12 );
                    r = true;
                }
            }
        }
        return r;
    }

    bool getDestinationForMessage( AddressType *addr, FixedBuffer const &payload, bool multicast ) const
    {
        bool r = false;

        // Get the destination multicast address if there is
        r = isMessageMulticast( addr, payload, multicast );

        if ( !r )
        {
            // it isn't multicast, try find the entity_id from it
            Eui64 entity_id;
            r = getRemoteEntityIdFromSentAVTPDU( &entity_id, payload );

            if ( r )
            {
                // got a target entity_id, see if we have an address for it
                auto const &i = m_known_entity_addresses.find( entity_id );
                if ( i != m_known_entity_addresses.end() )
                {
                    EntityAddressInfo const &info = i->second;
                    *addr = info.m_remote_addr;
                    r = true;
                }
                else
                {
                    // no address, so don't send it.
                    r = false;
                }
            }
        }
        return r;
    }

  private:
    Milliseconds m_expiry_time_in_milliseconds;
    Milliseconds m_flush_period_in_milliseconds;
    Milliseconds m_last_flush_time;

    std::map<Eui64, EntityAddressInfo> m_known_entity_addresses;
};
}
