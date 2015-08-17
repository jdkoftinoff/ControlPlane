#pragma once

#include "World.hpp"
#include "AvdeccSchema.hpp"
#include "ControlIdentity.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{

class ChangeNotificationRange
{
  public:
    ChangeNotificationRange( Milliseconds max_update_period_in_milliseconds = Milliseconds( 30 ),
                             Milliseconds min_update_period_in_milliseconds = Milliseconds( 10000 ) )
    {
        for ( auto &i : m_descriptors )
        {
            i = true;
        }
        m_max_update_period_in_milliseconds = max_update_period_in_milliseconds;
        m_min_update_period_in_milliseconds = min_update_period_in_milliseconds;
        m_min_descriptor_index = 0;
        m_max_descriptor_index = AVDECC_NUM_DESCRIPTOR_TYPES;
    }

    ChangeNotificationRange( ControlIdentity identity,
                             Milliseconds max_update_period_in_milliseconds = Milliseconds( 30 ),
                             Milliseconds min_update_period_in_milliseconds = Milliseconds( 10000 ) )
    {
        for ( auto &i : m_descriptors )
        {
            i = false;
        }
        m_descriptors[identity.m_descriptor_type] = true;
        m_min_descriptor_index = identity.m_descriptor_index;
        m_max_descriptor_index = identity.m_descriptor_index;
        m_max_update_period_in_milliseconds = max_update_period_in_milliseconds;
        m_min_update_period_in_milliseconds = min_update_period_in_milliseconds;
    }

    void setAll()
    {
        for ( auto &i : m_descriptors )
        {
            i = true;
        }
        m_min_descriptor_index = 0;
        m_max_descriptor_index = AVDECC_NUM_DESCRIPTOR_TYPES;
    }

    std::array<bool, AVDECC_NUM_DESCRIPTOR_TYPES> m_descriptors;
    uint16_t m_min_descriptor_index;
    uint16_t m_max_descriptor_index;

    Milliseconds m_max_update_period_in_milliseconds;
    Milliseconds m_min_update_period_in_milliseconds;

    bool containsDescriptor( ControlIdentity const &d ) const;

    friend bool operator==( ChangeNotificationRange const &lhs, ChangeNotificationRange const &rhs );

    friend bool operator<( ChangeNotificationRange const &lhs, ChangeNotificationRange const &rhs );

    friend std::ostream &operator<<( std::ostream &o, ChangeNotificationRange const &v );
};
}
