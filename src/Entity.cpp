#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Entity.hpp"
#include "ControlPlane/Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

Entity::Entity( std::string description,
                DescriptorString *entity_name,
                DescriptorString *group_name,
                DescriptorString *firmware_version,
                DescriptorString *serial_number,
                RangedValueEUI64 *entity_id,
                RangedValueEUI64 *entity_model_id )
    : m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_ENTITY ), m_description( description )
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

const DescriptorString *Entity::getName( size_t name_index ) const
{
    DescriptorString *r = m_names[name_index];

    return r;
}

DescriptorString *Entity::getName( size_t name_index )
{
    DescriptorString *r = m_names[name_index];

    return r;
}

bool Entity::setName( std::string val, size_t name_index )
{
    DescriptorString *r = m_names[name_index];

    return r->setValue( val );
}

ControlValue &Entity::getValue( size_t item_num, size_t w, size_t h )
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

const ControlValue &Entity::getValue( size_t item_num, size_t w, size_t h ) const
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

void Entity::fillWriteAccess( ControlIdentityComparatorSetPtr &write_access )
{
    write_access->addItem( getControlIdentityForItem( ItemForEntityId ) );
    write_access->addItem( getControlIdentityForItem( ItemForEntityModelId ) );
    write_access->addItem( getControlIdentityForItem( ItemForSerialNumber ) );
    write_access->addItem( getControlIdentityForItem( ItemForFirmwareVersion ) );
}

void Entity::storeToPDU( ControlPlane::FixedBuffer &pdu ) const
{
    pdu.putDoublet( m_avdecc_descriptor_type );
    pdu.putDoublet( m_avdecc_descriptor_index );
    pdu.putEUI64( m_items[ItemForEntityId].m_ranged_value->getUnencodedValueUInt64() );
    pdu.putEUI64( m_items[ItemForEntityModelId].m_ranged_value->getUnencodedValueUInt64() );
    pdu.putQuadlet( 0 );     // entity_capabilities
    pdu.putDoublet( 0 );     // talker_stream_sources
    pdu.putDoublet( 0 );     // talker_capabilities
    pdu.putDoublet( 0 );     // listener_stream_sinks
    pdu.putDoublet( 0 );     // listener_capabilities
    pdu.putQuadlet( 0 );     // controller_capabilities
    pdu.putQuadlet( 0 );     // available_index
    pdu.putEUI64( Eui64() ); // association_id
    pdu.putAvdeccString( m_items[ItemForName].m_ranged_value->getUnencodedValueString() );
    pdu.putDoublet( 0 ); // vendor_name_string
    pdu.putDoublet( 0 ); // model_name_string
    pdu.putAvdeccString( m_items[ItemForFirmwareVersion].m_ranged_value->getUnencodedValueString() );
    pdu.putAvdeccString( m_items[ItemForGroup].m_ranged_value->getUnencodedValueString() );
    pdu.putAvdeccString( m_items[ItemForSerialNumber].m_ranged_value->getUnencodedValueString() );
    pdu.putDoublet( (uint16_t)m_configurations.size() ); // configurations_count
    pdu.putDoublet( 0 );                                 // current_configuration
}

void Entity::collectOwnedDescriptors( DescriptorCounts &counts )
{
    DescriptorBase::collectOwnedDescriptors( counts );
    for ( auto &i : m_configurations )
    {
        i->collectOwnedDescriptors( counts );
        counts.addConfiguration();
    }
}
}
}
