#include "ControlPlane/World.hpp"
#include "ControlPlane/EntityDescriptor.hpp"
#include "ControlPlane/Values.hpp"

namespace ControlPlane
{

EntityDescriptor::EntityDescriptor( DescriptorCounts &counts,
                                    std::string description,
                                    DescriptorString *entity_name,
                                    DescriptorString *group_name,
                                    DescriptorString *firmware_version,
                                    DescriptorString *serial_number,
                                    EUI64 *entity_id,
                                    EUI64 *entity_model_id )
    : m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_ENTITY )
    , m_avdecc_descriptor_index( counts.getCountForDescriptorTypeAndIncrement( m_avdecc_descriptor_type ) )
    , m_description( description )
{
    m_names.push_back( entity_name );
    m_names.push_back( group_name );
    m_items.push_back( ControlValue{"name", entity_name} );
    m_items.push_back( ControlValue{"group", group_name} );
    m_items.push_back( ControlValue{"firmware_version", firmware_version} );
    m_items.push_back( ControlValue{"serial_number", serial_number} );
    m_items.push_back( ControlValue{"entity_id", entity_id} );
    m_items.push_back( ControlValue{"entity_model_id", entity_model_id} );
}

const DescriptorString *EntityDescriptor::getName( size_t name_index ) const
{
    DescriptorString *r = m_names[name_index];

    return r;
}

DescriptorString *EntityDescriptor::getName( size_t name_index )
{
    DescriptorString *r = m_names[name_index];

    return r;
}

bool EntityDescriptor::setName( std::string val, size_t name_index )
{
    DescriptorString *r = m_names[name_index];

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

void EntityDescriptor::fillWriteAccess( ControlIdentityComparatorSetPtr &write_access )
{
    write_access->addItem( getControlIdentityForItem( ItemForEntityId ) );
    write_access->addItem( getControlIdentityForItem( ItemForEntityModelId ) );
    write_access->addItem( getControlIdentityForItem( ItemForSerialNumber ) );
    write_access->addItem( getControlIdentityForItem( ItemForFirmwareVersion ) );
}
}
