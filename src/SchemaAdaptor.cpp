#include "ControlPlane/World.hpp"
#include "ControlPlane/SchemaAdaptor.hpp"

namespace ControlPlane
{

void SchemaAdaptorBase::collectDescriptor( const SchemaAddress &schema_address,
                                           DescriptorPtr descriptor,
                                           ControlIdentity identity )
{
    collectItem( identity, schema_address );
}

void SchemaAdaptorBase::collectDescriptors()
{
    getTop()->enumerate( [=]( const SchemaAddress &schema_address, DescriptorPtr descriptor, ControlIdentity identity )
                         {
                             collectDescriptor( schema_address, descriptor, identity );
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
