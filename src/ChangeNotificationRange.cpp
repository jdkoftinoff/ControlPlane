#include "ControlPlane/World.hpp"
#include "ControlPlane/ChangeNotificationRange.hpp"

namespace ControlPlane
{

bool operator==( ChangeNotificationRange const &lhs, ChangeNotificationRange const &rhs )
{
    bool r = false;
    if ( lhs.m_descriptors == rhs.m_descriptors )
    {
        if ( lhs.m_min_descriptor_index <= rhs.m_min_descriptor_index )
        {
            if ( lhs.m_max_descriptor_index >= rhs.m_max_descriptor_index )
            {
                r = true;
            }
        }
    }
    return r;
}

bool operator<( ChangeNotificationRange const &lhs, ChangeNotificationRange const &rhs )
{
    bool r = false;
    if ( lhs.m_descriptors < rhs.m_descriptors )
    {
        r = true;
    }
    else if ( lhs.m_descriptors == rhs.m_descriptors )
    {
        if ( lhs.m_min_descriptor_index < rhs.m_min_descriptor_index )
        {
            r = true;
        }
        else if ( ( lhs.m_min_descriptor_index == rhs.m_min_descriptor_index )
                  && ( lhs.m_max_descriptor_index < rhs.m_max_descriptor_index ) )
        {
            r = true;
        }
    }
    return r;
}

bool ChangeNotificationRange::containsDescriptor( const ControlIdentity &d ) const
{
    bool r = false;
    if ( m_descriptors[d.m_descriptor_type] == true )
    {
        if ( d.m_descriptor_index >= m_min_descriptor_index && d.m_descriptor_index <= m_max_descriptor_index )
        {
            r = true;
        }
    }
    return r;
}

std::ostream &operator<<( std::ostream &o, ChangeNotificationRange const &v )
{
    for ( size_t i = 0; i < v.m_descriptors.size(); ++i )
    {
        if ( v.m_descriptors[i] )
        {
            o << "Descriptor Type " << i << std::endl;
        }
    }

    o << "min_descriptor_index: " << v.m_min_descriptor_index << std::endl;
    o << "max_descriptor_index: " << v.m_max_descriptor_index << std::endl;

    o << "min_update_period_in_milliseconds: " << v.m_min_update_period_in_milliseconds << std::endl;
    o << "max_update_period_in_milliseconds: " << v.m_max_update_period_in_milliseconds << std::endl;

    return o;
}
}
