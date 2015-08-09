#include "ControlPlane/World.hpp"
#include "ControlPlane/ControlDescriptor.hpp"

namespace ControlPlane
{

ControlDescriptor::ControlDescriptor( DescriptorCounts &counts,
                                      uint64_t avdecc_control_type,
                                      std::string description,
                                      uint16_t avdecc_control_value_type,
                                      ControlValue first )
    : m_avdecc_control_type( avdecc_control_type )
    , m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_CONTROL )
    , m_avdecc_descriptor_index( counts.getCountForDescriptorTypeAndIncrement( m_avdecc_descriptor_type ) )
    , m_description( description )
    , m_avdecc_control_value_type( avdecc_control_value_type )
{
    push_back_multi( m_control_point_values, first );
}

const DescriptorObjectName *ControlDescriptor::getName( size_t name_index ) const
{
    DescriptorObjectName const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorObjectName *ControlDescriptor::getName( size_t name_index )
{
    DescriptorObjectName *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool ControlDescriptor::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}
