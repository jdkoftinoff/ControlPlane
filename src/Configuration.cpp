#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Configuration.hpp"
#include "ControlPlane/Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

template <typename T>
void putDescriptorCounts( FixedBuffer &pdu, const std::vector<std::shared_ptr<T> > &items )
{
    if ( items.size() > 0 )
    {
        pdu.putDoublet( items[0]->getAvdeccDescriptorType() );
        pdu.putDoublet( (uint16_t)items.size() );
    }
}

void Configuration::storeToPDU( FixedBuffer &pdu ) const
{
    pdu.putDoublet( getAvdeccDescriptorType() );
    pdu.putDoublet( getAvdeccDescriptorIndex() );
    pdu.putAvdeccString( getName( "object_name" ).m_ranged_value->getUnencodedValueString() );
    pdu.putDoublet( 0xffff );

    size_t descriptor_counts_count = getChildDescriptors().size();

    pdu.putDoublet( (uint16_t)descriptor_counts_count );
    pdu.putDoublet( 74 );

    for ( auto const &i : getChildDescriptors() )
    {
        putDescriptorCounts( pdu, i.second );
    }
}
}
}
