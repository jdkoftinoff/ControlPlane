#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Descriptor.hpp"

namespace ControlPlane
{
namespace Descriptor
{
void DescriptorBase::collectOwnedDescriptors( DescriptorCounts &counts )
{
    setAvdeccDescriptorIndex( counts.getCountForDescriptorTypeAndIncrement( getAvdeccDescriptorType() ) );

    for ( auto &v : m_child_descriptors )
    {
        for ( auto &i : v.second )
        {
            i->collectOwnedDescriptors( counts );
        }
    }
}

void DescriptorBase::addChildDescriptor( std::shared_ptr<DescriptorBase> d )
{
    auto const &v = m_child_descriptors.find( d->getAvdeccDescriptorType() );
    if ( v == m_child_descriptors.end() )
    {
        m_child_descriptors[d->getAvdeccDescriptorType()] = std::vector<std::shared_ptr<DescriptorBase> >();
    }

    m_child_descriptors[d->getAvdeccDescriptorType()].push_back( d );
}

size_t DescriptorBase::getNumChildDescriptorsForType( uint16_t type ) const
{
    size_t s = 0;

    auto const &v = m_child_descriptors.find( type );
    if ( v != m_child_descriptors.end() )
    {
        s = v->second.size();
    }
    return s;
}

std::vector<std::shared_ptr<DescriptorBase> > *DescriptorBase::getChildDescriptorsForType( uint16_t type )
{
    std::vector<std::shared_ptr<DescriptorBase> > *r = 0;
    auto const &v = m_child_descriptors.find( type );
    if ( v != m_child_descriptors.end() )
    {
        r = &v->second;
    }
    return r;
}

const std::vector<std::shared_ptr<DescriptorBase> > *DescriptorBase::getChildDescriptorsForType( uint16_t type ) const
{
    std::vector<std::shared_ptr<DescriptorBase> > const *r = 0;
    auto const &v = m_child_descriptors.find( type );
    if ( v != m_child_descriptors.end() )
    {
        r = &v->second;
    }
    return r;
}
}
}
