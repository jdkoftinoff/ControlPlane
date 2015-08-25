#include "ControlPlane/World.hpp"
#include "ControlPlane/Schema.hpp"

namespace ControlPlane
{
using Util::formstring;

void Schema::collectDescriptors()
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    m_top_level->enumerate(
        [=]( const SchemaAddress &address, DescriptorPtr descriptor, ControlIdentity identity )
        {
            SchemaAddress sub = address;

            for ( uint16_t i = 0; i < descriptor->getNumProperties(); ++i )
            {
                sub.push_back( descriptor->getPropertyName( i ) );
                m_top_level->addItem( sub, descriptor, descriptor->getControlIdentityForProperty( i ) );
                sub.pop_back();
            }
            if ( descriptor->getHeight() > 1 && descriptor->getWidth() > 1 && descriptor->getNumValues() > 0 )
            {
                for ( uint16_t h = 0; h < descriptor->getHeight(); ++h )
                {
                    sub.push_back( formstring( h + 1 ) );

                    for ( uint16_t w = 0; w < descriptor->getWidth(); ++w )
                    {
                        sub.push_back( formstring( w + 1 ) );

                        if ( descriptor->getNumValues() == 1 )
                        {
                            m_top_level->addItem( sub, descriptor, descriptor->getControlIdentityForItem( 0, h, w ) );
                        }
                        else
                        {
                            for ( uint16_t i = 0; i < descriptor->getNumValues(); ++i )
                            {
                                sub.push_back( formstring( i + 1 ) );
                                m_top_level->addItem( sub, descriptor, descriptor->getControlIdentityForItem( i, h, w ) );
                                sub.pop_back();
                            }
                        }
                        sub.pop_back();
                    }
                    sub.pop_back();
                }
            }
            else
            {
                if ( descriptor->getHeight() == 1 && descriptor->getWidth() == 1 && descriptor->getNumValues() > 1 )
                {
                    for ( uint16_t i = 0; i < descriptor->getNumValues(); ++i )
                    {
                        sub.push_back( formstring( i + 1 ) );
                        m_top_level->addItem( sub, descriptor, descriptor->getControlIdentityForItem( i, 0, 0 ) );
                        sub.pop_back();
                    }
                }
                else
                {
                    if ( descriptor->getHeight() == 1 && descriptor->getWidth() == 1 && descriptor->getNumValues() == 1 )
                    {
                        m_top_level->addItem( sub, descriptor, descriptor->getControlIdentityForItem( 0, 0, 0 ) );
                    }
                }
            }

        } );

    m_top_level->updateControlIdentities();

    m_top_level->enumerate( [=]( const SchemaAddress &address, DescriptorPtr descriptor, ControlIdentity identity )
                            {
                                ControlIdentity descriptor_identity = identity;
                                descriptor_identity.m_section = ControlIdentity::SectionDescriptorLevel;
                                descriptor_identity.m_h_pos = 0;
                                descriptor_identity.m_w_pos = 0;
                                descriptor_identity.m_item = 0;
                                m_descriptor_avdecc_map[descriptor_identity] = descriptor;
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
        r = d->getName( item_num + identity.m_item ).m_ranged_value;
        break;

    case ControlIdentity::SectionProperty:
        r = d->getProperty( item_num + identity.m_item ).m_ranged_value;
        break;

    case ControlIdentity::SectionDescriptorLevel:
    case ControlIdentity::SectionWPosLevel:
    case ControlIdentity::SectionHPosLevel:
        if ( d->getNumValues() > item_num + identity.m_item )
        {
            if ( d->getHeight() > h_pos + identity.m_h_pos )
            {
                if ( d->getWidth() > w_pos + identity.m_w_pos )
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
            r = d->getName( 0 ).m_ranged_value;
        }
        else
        {
            if ( d->getNumProperties() > 0 )
            {
                r = d->getProperty( 0 ).m_ranged_value;
            }
            else
            {
                throw SchemaErrorNoSuchControlIdentity( identity );
            }
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
        case ControlIdentity::SectionProperty:
            r = d->getProperty( item_num ).m_ranged_value;
            break;

        case ControlIdentity::SectionName:
            r = d->getName( item_num ).m_ranged_value;
            ;
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
                r = d->getName( 0 ).m_ranged_value;
                ;
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
