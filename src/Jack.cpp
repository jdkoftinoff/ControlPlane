#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Jack.hpp"
#include "ControlPlane/Descriptor/Control.hpp"

namespace ControlPlane
{
namespace Descriptor
{

const DescriptorString *Jack::getName( size_t name_index ) const
{
    DescriptorString const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorString *Jack::getName( size_t name_index )
{
    DescriptorString *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool Jack::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}

void Jack::storeToPDU( FixedBuffer &pdu ) const {}

void Jack::collectOwnedDescriptors( DescriptorCounts &counts )
{
    DescriptorBase::collectOwnedDescriptors( counts );
    for ( auto &i : m_controls )
    {
        i->collectOwnedDescriptors( counts );
    }
}
}
}
