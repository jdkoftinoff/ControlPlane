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
                            } );
}

const DescriptorPtr Schema::getDescriptor( const ControlIdentity &identity ) const
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    DescriptorPtr r;
    auto i = m_descriptor_avdecc_map.find( identity );
    if ( i != m_descriptor_avdecc_map.end() )
    {
        r = i->second;
    }
    else
    {
        throw std::runtime_error(
            formstring( "No such descriptor: ", identity.m_descriptor_type, ",", identity.m_descriptor_index ) );
    }
    return r;
}

DescriptorPtr Schema::getDescriptor( const ControlIdentity &identity )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    DescriptorPtr r;
    auto i = m_descriptor_avdecc_map.find( identity );
    if ( i != m_descriptor_avdecc_map.end() )
    {
        r = i->second;
    }
    else
    {
        throw std::runtime_error(
            formstring( "No such descriptor: ", identity.m_descriptor_type, ",", identity.m_descriptor_index ) );
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
        string a;
        for ( auto &elem : address )
        {
            a.append( "" );
            a.append( elem );
        }
        throw std::runtime_error( formstring( "No descriptor identity for address:", a ) );
    }
    return r;
}

RangedValueBase const *
    Schema::getRangedValueForDescriptor( ControlIdentity const &identity, int item_num, int w_pos, int h_pos ) const
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    RangedValueBase *r = 0;
    DescriptorPtr d = getDescriptor( identity );

    switch ( identity.m_section )
    {
    case ControlIdentity::SectionName:
        r = d->getName( item_num );
        break;

    case ControlIdentity::SectionItemLevel:
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
        throw std::runtime_error(
            formstring( "No such control value: ", identity.m_descriptor_type, ",", identity.m_descriptor_index ) );
    }
    return r;
}

RangedValueBase *Schema::getRangedValueForDescriptor( ControlIdentity const &identity, int item_num, int w_pos, int h_pos )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    RangedValueBase *r = 0;
    DescriptorPtr d = getDescriptor( identity );

    switch ( identity.m_section )
    {
    case ControlIdentity::SectionName:
        r = d->getName( item_num );
        break;

    case ControlIdentity::SectionItemLevel:
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
        throw std::runtime_error(
            formstring( "No such control value: ", identity.m_descriptor_type, ",", identity.m_descriptor_index ) );
    }
    return r;
}
}
