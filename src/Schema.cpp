#include "ControlPlane/World.hpp"
#include "ControlPlane/Schema.hpp"

namespace ControlPlane
{
using Util::formstring;

void Schema::collectDescriptors()
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    m_top_level->enumerate( [=]( const SchemaAddress &address, DescriptorPtr descriptor )
                            {
                                ControlIdentity identity = descriptor->getControlIdentity();
                                m_descriptor_avdecc_map[identity] = descriptor;
                                m_address_map[address] = identity;
                                m_identity_map[identity] = address;
                            } );
}

const DescriptorPtr Schema::getDescriptor( const ControlIdentity &requested_identity ) const
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    ControlIdentity identity = requested_identity;
    identity.m_section = ControlIdentity::SectionDescriptorLevel;
    identity.m_item = 0;
    identity.m_w_pos = 0;
    identity.m_h_pos = 0;

    DescriptorPtr r;
    auto i = m_descriptor_avdecc_map.find( identity );
    if ( i != m_descriptor_avdecc_map.end() )
    {
        r = i->second;
    }
    else
    {
        throw SchemaErrorNoSuchDescriptor( identity.m_descriptor_type, identity.m_descriptor_index );
    }
    return r;
}

DescriptorPtr Schema::getDescriptor( const ControlIdentity &requested_identity )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    ControlIdentity identity = requested_identity;
    identity.m_section = ControlIdentity::SectionDescriptorLevel;
    identity.m_item = 0;
    identity.m_w_pos = 0;
    identity.m_h_pos = 0;

    DescriptorPtr r;
    auto i = m_descriptor_avdecc_map.find( identity );
    if ( i != m_descriptor_avdecc_map.end() )
    {
        r = i->second;
    }
    else
    {
        throw SchemaErrorNoSuchDescriptor( identity.m_descriptor_type, identity.m_descriptor_index );
    }
    return r;
}

ControlIdentity Schema::getIdentityForAddress( const SchemaAddress &address ) const
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    ControlIdentity r;
    auto i = m_address_map.find( address );
    if ( i != m_address_map.end() )
    {
        r = i->second;
    }
    else
    {
        throw SchemaErrorNoSuchDescriptorForAddress( address );
    }
    return r;
}

RangedValueBase const *
    Schema::getRangedValueForControlIdentity( ControlIdentity const &identity, int item_num, int w_pos, int h_pos ) const
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    RangedValueBase *r = 0;
    DescriptorPtr d = getDescriptor( identity );

    switch ( identity.m_section )
    {
    case ControlIdentity::SectionName:
        r = d->getName( item_num );
        break;

    case ControlIdentity::SectionDescriptorLevel:
    case ControlIdentity::SectionWPosLevel:
    case ControlIdentity::SectionHPosLevel:
        if ( d->getNumValues() > item_num )
        {
            if ( d->getHeight() > h_pos )
            {
                if ( d->getWidth() > w_pos )
                {
                    r = d->getValue( item_num + identity.m_item, w_pos + identity.m_w_pos, h_pos + identity.m_h_pos )
                            .m_ranged_value;
                }
            }
        }
        break;
    default:
        r = 0;
        break;
    }

    if ( !r )
    {
        if ( d->getNumNames() > 0 )
        {
            r = d->getName( 0 );
        }
        else
        {
            throw SchemaErrorNoSuchControlIdentity( identity );
        }
    }

    return r;
}

RangedValueBase *Schema::getRangedValueForControlIdentity(
    ControlIdentityComparatorPtr write_validator, ControlIdentity const &identity, int item_num, int w_pos, int h_pos )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    RangedValueBase *r = 0;
    bool write_access_allowed = true;

    if ( write_validator )
    {
        if ( write_validator->containsControl( identity ) )
        {
            write_access_allowed = false;
        }
    }

    if ( write_access_allowed )
    {
        DescriptorPtr d = getDescriptor( identity );

        switch ( identity.m_section )
        {
        case ControlIdentity::SectionName:
            r = d->getName( item_num );
            break;

        case ControlIdentity::SectionDescriptorLevel:
        case ControlIdentity::SectionWPosLevel:
        case ControlIdentity::SectionHPosLevel:
            if ( d->getNumValues() > item_num )
            {
                if ( d->getHeight() > h_pos )
                {
                    if ( d->getWidth() > w_pos )
                    {
                        r = d->getValue( item_num + identity.m_item, w_pos + identity.m_w_pos, h_pos + identity.m_h_pos )
                                .m_ranged_value;
                    }
                }
            }
            break;
        default:
            r = 0;
            break;
        }

        if ( !r )
        {
            if ( d->getNumNames() > 0 )
            {
                r = d->getName( 0 );
            }
            else
            {
                throw SchemaErrorNoSuchControlIdentity( identity );
            }
        }
    }
    else
    {
        throw SchemaErrorReadOnly( identity );
    }
    return r;
}
}
