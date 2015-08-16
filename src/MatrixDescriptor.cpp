#include "ControlPlane/World.hpp"
#include "ControlPlane/MatrixDescriptor.hpp"

namespace ControlPlane
{

MatrixDescriptor::MatrixDescriptor( DescriptorCounts &counts,
                                    uint64_t avdecc_control_type,
                                    std::string description,
                                    uint16_t avdecc_control_value_type )
    : m_avdecc_control_type( avdecc_control_type )
    , m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_MATRIX )
    , m_avdecc_descriptor_index( counts.getCountForDescriptorTypeAndIncrement( m_avdecc_descriptor_type ) )
    , m_description( description )
    , m_avdecc_control_value_type( avdecc_control_value_type )
{
}

const DescriptorString *MatrixDescriptor::getName( size_t name_index ) const
{
    DescriptorString const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorString *MatrixDescriptor::getName( size_t name_index )
{
    DescriptorString *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool MatrixDescriptor::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}