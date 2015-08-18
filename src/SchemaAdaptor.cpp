#include "ControlPlane/World.hpp"
#include "ControlPlane/SchemaAdaptor.hpp"

namespace ControlPlane
{

void SchemaAdaptorBase::collectDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor )
{
    ControlIdentity identity = descriptor->getControlIdentity();
    collectItem( identity, schema_address );

    uint16_t descriptor_type = descriptor->getAvdeccDescriptorType();

    switch ( descriptor_type )
    {
    case AVDECC_DESCRIPTOR_ENTITY:
        collectEntityDescriptor( schema_address, descriptor );
        break;
    case AVDECC_DESCRIPTOR_MATRIX:
        collectMatrixDescriptor( schema_address, descriptor );
        break;
    case AVDECC_DESCRIPTOR_CONTROL:
        collectControlDescriptor( schema_address, descriptor );
        break;
    default:
        collectOtherDescriptor( schema_address, descriptor );
        break;
    }
}

void SchemaAdaptorBase::collectEntityDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor )
{
    {
        for ( uint16_t i = 0; i < descriptor->getNumValues(); ++i )
        {
            ControlValue const &value = descriptor->getValue( i );
            ControlIdentity identity = descriptor->getControlIdentityForItem( i );
            SchemaAddress item_schema_address = schema_address;
            item_schema_address.push_back( value.m_name );
            collectItem( identity, item_schema_address );
        }
    }
}

void SchemaAdaptorBase::collectMatrixDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor )
{
    uint16_t width = descriptor->getWidth();
    uint16_t height = descriptor->getHeight();

    for ( uint16_t h_pos = 0; h_pos < height; ++h_pos )
    {
        SchemaAddress row_schema_address = schema_address;
        row_schema_address.push_back( Util::formstring( h_pos + 1 ) );

        ControlIdentity row_identity = descriptor->getControlIdentityForItem( 0, h_pos );
        collectItem( row_identity, row_schema_address );

        for ( uint16_t w_pos = 0; w_pos < width; ++w_pos )
        {
            SchemaAddress item_schema_address = row_schema_address;
            item_schema_address.push_back( Util::formstring( w_pos + 1 ) );
            ControlIdentity item_identity = descriptor->getControlIdentityForItem( 0, w_pos, h_pos );

            collectItem( item_identity, item_schema_address );
        }
    }
}

void SchemaAdaptorBase::collectControlDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor )
{
    // Collect the value mapping if there is more than 1
    uint16_t num_values = descriptor->getNumValues();
    if ( num_values > 1 )
    {
        for ( uint16_t item_num = 0; item_num < num_values; ++item_num )
        {
            ControlIdentity identity = descriptor->getControlIdentityForItem( item_num );
            SchemaAddress item_schema_address = schema_address;
            item_schema_address.push_back( Util::formstring( item_num + 1 ) );

            collectItem( identity, item_schema_address );
        }
    }
}

void SchemaAdaptorBase::collectOtherDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor )
{
    // Collect the top level mapping
    ControlIdentity identity = descriptor->getControlIdentity();

    uint16_t num_names = descriptor->getNumNames();
    for ( uint16_t name_num = 0; name_num < num_names; ++name_num )
    {
        SchemaAddress name_schema_address = schema_address;
        name_schema_address.push_back( "name" );
        if ( num_names > 1 )
        {
            name_schema_address.push_back( Util::formstring( name_num + 1 ) );
        }
        collectItem( identity, name_schema_address );
    }
}

void SchemaAdaptorBase::collectDescriptors()
{
    getTop()->enumerate( [=]( const SchemaAddress &schema_address, DescriptorPtr descriptor )
                         {
                             collectDescriptor( schema_address, descriptor );
                         } );
}

bool getAddressForIdentity( ControlIdentity &address,
                            const ControlIdentity &identity,
                            const SchemaAddress &schema_address,
                            Schema &schema )
{
    address = identity;
    return true;
}
}
