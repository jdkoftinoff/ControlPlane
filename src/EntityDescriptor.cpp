#include "ControlPlane/World.hpp"
#include "ControlPlane/EntityDescriptor.hpp"

namespace ControlPlane
{

EntityDescriptor::EntityDescriptor( DescriptorCounts &counts, std::string description )
    : m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_ENTITY )
    , m_avdecc_descriptor_index( counts.getCountForDescriptorTypeAndIncrement( m_avdecc_descriptor_type ) )
    , m_description( description )

{
    m_items.push_back( ControlValue{"firmware_version", &m_firmware_version} );
    m_items.push_back( ControlValue{"serial_number", &m_serial_number} );
}

const DescriptorObjectName *EntityDescriptor::getName( size_t name_index ) const
{
    DescriptorObjectName const *r = &m_names[name_index];

    return r;
}

DescriptorObjectName *EntityDescriptor::getName( size_t name_index )
{
    DescriptorObjectName *r = &m_names[name_index];

    return r;
}

bool EntityDescriptor::setName( std::string val, size_t name_index )
{
    DescriptorObjectName *r = &m_names[name_index];

    return r->setValue( val );
}

ControlValue &EntityDescriptor::getValue( size_t item_num, size_t w, size_t h )
{
    if ( w == 0 && h == 0 )
    {
        return m_items[item_num];
    }
    else
    {
        throw std::runtime_error( Util::formstring( "Invalid entity item: (", item_num, ",", w, ",", h, ")" ) );
    }
}

const ControlValue &EntityDescriptor::getValue( size_t item_num, size_t w, size_t h ) const
{
    if ( w == 0 && h == 0 )
    {
        return m_items[item_num];
    }
    else
    {
        throw std::runtime_error( Util::formstring( "Invalid entity item: (", item_num, ",", w, ",", h, ")" ) );
    }
}
}
