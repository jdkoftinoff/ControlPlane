#pragma once

#include "World.hpp"
#include "ControlValue.hpp"

namespace ControlPlane
{

struct ControlIdentity
{
    enum Section
    {
        SectionName,
        SectionItemLevel,
        SectionWPosLevel,
        SectionHPosLevel
    };

    ControlIdentity()
        : m_descriptor_type( 0xffff )
        , m_descriptor_index( 0x0000 )
        , m_section( SectionItemLevel )
        , m_item( 0 )
        , m_w_pos( 0 )
        , m_h_pos( 0 )
    {
    }

    ControlIdentity( uint16_t descriptor_type,
                     uint16_t descriptor_index,
                     Section section = SectionItemLevel,
                     uint16_t item = 0,
                     uint16_t w_pos = 0,
                     uint16_t h_pos = 0 )
        : m_descriptor_type( descriptor_type )
        , m_descriptor_index( descriptor_index )
        , m_section( section )
        , m_item( item )
        , m_w_pos( w_pos )
        , m_h_pos( h_pos )
    {
    }

    uint16_t m_descriptor_type;
    uint16_t m_descriptor_index;
    Section m_section;
    uint16_t m_item;
    uint16_t m_w_pos;
    uint16_t m_h_pos;
};

inline std::ostream &operator<<( std::ostream &o, ControlIdentity const &v )
{
    o << "{ " << v.m_descriptor_type << ", " << v.m_descriptor_index << ", " << v.m_section << ", " << v.m_item << ", "
      << v.m_w_pos << ", " << v.m_h_pos << " }";

    return o;
}

int compare( ControlIdentity const &lhs, ControlIdentity const &rhs );

inline bool operator<( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) < 0; }

inline bool operator<=( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) <= 0; }

inline bool operator==( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) == 0; }

inline bool operator>=( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) >= 0; }

inline bool operator>( ControlIdentity const &lhs, ControlIdentity const &rhs ) { return compare( lhs, rhs ) > 0; }
}
