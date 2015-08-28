#include "ControlPlane/World.hpp"
#include "ControlPlane/InternalAddress.hpp"

namespace ControlPlane
{

bool getInternalAddressForIdentity( InternalAddress &address,
                                    const ControlIdentity &identity,
                                    const SchemaAddress &schema_address,
                                    Schema &schema )
{
    const RangedValueBase *r = schema.getRangedValueForControlIdentity( identity );
    address = InternalAddress( r );
    return r != nullptr;
}
}
