#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Descriptor.hpp"
#include "ControlPlane/Schema.hpp"

namespace ControlPlane
{
namespace Descriptor
{

void DescriptorBase::collectOwnedDescriptors( DescriptorCounts &counts, ControlContainerPtr &top )
{
    setAvdeccDescriptorIndex( counts.getCountForDescriptorTypeAndIncrement( getAvdeccDescriptorType() ) );

    for ( auto &v : m_child_descriptor_map )
    {
        for ( auto &i : v.second )
        {
            i->collectOwnedDescriptors( counts, top );
        }
    }
}

void DescriptorBase::addChildDescriptor( std::shared_ptr<DescriptorBase> d )
{
    auto const &v = m_child_descriptor_map.find( d->getAvdeccDescriptorType() );
    if ( v == m_child_descriptor_map.end() )
    {
        m_child_descriptor_map[d->getAvdeccDescriptorType()] = std::vector<std::shared_ptr<DescriptorBase> >();
    }

    m_child_descriptor_map[d->getAvdeccDescriptorType()].push_back( d );
}

size_t DescriptorBase::getNumChildDescriptorsForType( uint16_t type ) const
{
    size_t s = 0;

    auto const &v = m_child_descriptor_map.find( type );
    if ( v != m_child_descriptor_map.end() )
    {
        s = v->second.size();
    }
    return s;
}

std::vector<std::shared_ptr<DescriptorBase> > *DescriptorBase::getChildDescriptorsForType( uint16_t type )
{
    std::vector<std::shared_ptr<DescriptorBase> > *r = 0;
    auto const &v = m_child_descriptor_map.find( type );
    if ( v != m_child_descriptor_map.end() )
    {
        r = &v->second;
    }
    return r;
}

const std::vector<std::shared_ptr<DescriptorBase> > *DescriptorBase::getChildDescriptorsForType( uint16_t type ) const
{
    std::vector<std::shared_ptr<DescriptorBase> > const *r = 0;
    auto const &v = m_child_descriptor_map.find( type );
    if ( v != m_child_descriptor_map.end() )
    {
        r = &v->second;
    }
    return r;
}
}
}
