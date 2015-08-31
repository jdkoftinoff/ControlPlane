#pragma once

#include "World.hpp"
#include "ControlValue.hpp"
#include "Avdecc.hpp"

namespace ControlPlane
{

struct ControlIdentity
{
    enum Section
    {
        SectionDescriptorLevel,
        SectionProperty,
        SectionName,
        SectionHPosLevel,
        SectionWPosLevel
    };

    ControlIdentity()
        : m_descriptor_type( 0xffff )
        , m_descriptor_index( 0x0000 )
        , m_section( SectionDescriptorLevel )
        , m_item( 0 )
        , m_h_pos( 0 )
        , m_w_pos( 0 )
    {
    }

    ControlIdentity( DescriptorType descriptor_type,
                     DescriptorIndex descriptor_index,
                     Section section = SectionDescriptorLevel,
                     uint16_t item = 0,
                     uint16_t h_pos = 0,
                     uint16_t w_pos = 0 )
        : m_descriptor_type( descriptor_type )
        , m_descriptor_index( descriptor_index )
        , m_section( section )
        , m_item( item )
        , m_h_pos( h_pos )
        , m_w_pos( w_pos )
    {
    }

    ControlIdentity( const ControlIdentity &other ) = default;

    ControlIdentity &operator=( const ControlIdentity &other ) = default;

    DescriptorType m_descriptor_type;
    DescriptorIndex m_descriptor_index;
    Section m_section;
    uint16_t m_item;
    uint16_t m_h_pos;
    uint16_t m_w_pos;
};

inline std::ostream &operator<<( std::ostream &o, ControlIdentity const &v )
{
    o << "{ " << v.m_descriptor_type << ", " << v.m_descriptor_index << ", " << v.m_section << ", " << v.m_item << ", "
      << v.m_h_pos << ", " << v.m_w_pos << " }";

    return o;
}

int compare( ControlIdentity const &lhs, ControlIdentity const &rhs );

inline bool operator<( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) < 0; }

inline bool operator<=( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) <= 0; }

inline bool operator==( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) == 0; }

inline bool operator>=( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) >= 0; }

inline bool operator>( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) > 0; }
}
