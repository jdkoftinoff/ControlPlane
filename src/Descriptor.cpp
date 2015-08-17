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

ControlIdentity DescriptorBase::getControlIdentity() const
{
    ControlIdentity r = ControlIdentity{getAvdeccDescriptorType(), getAvdeccDescriptorIndex()};
    return r;
}

ControlIdentity DescriptorBase::getControlIdentityForItem( uint16_t item_num ) const
{
    ControlIdentity r
        = ControlIdentity( getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionWPosLevel, item_num );
    return r;
}

ControlIdentity DescriptorBase::getControlIdentityForItem( uint16_t item_num, uint16_t h_pos ) const
{
    ControlIdentity r = ControlIdentity(
        getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionWPosLevel, item_num, h_pos );
    return r;
}

ControlIdentity DescriptorBase::getControlIdentityForItem( uint16_t item_num, uint16_t h_pos, uint16_t w_pos ) const
{
    ControlIdentity r = ControlIdentity(
        getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionHPosLevel, item_num, h_pos, w_pos );
    return r;
}

ControlIdentity DescriptorBase::getControlIdentityForName( uint16_t name_num ) const
{
    ControlIdentity r
        = ControlIdentity( getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionName, name_num );
    return r;
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
