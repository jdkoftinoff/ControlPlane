#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Control.hpp"

namespace ControlPlane
{
namespace Descriptor
{

Control::Control( uint64_t avdecc_control_type,
                  std::string description,
                  uint16_t avdecc_control_value_type,
                  ControlValue first )
    : m_avdecc_control_type( avdecc_control_type )
    , m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_CONTROL )
    , m_description( description )
    , m_avdecc_control_value_type( avdecc_control_value_type )
{
    push_back_multi( m_control_point_values, first );
}

const DescriptorString *Control::getName( size_t name_index ) const
{
    DescriptorString const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorString *Control::getName( size_t name_index )
{
    DescriptorString *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool Control::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}
}
