#pragma once

#include "World.hpp"
#include <regex>

#include "SchemaAdaptor.hpp"

namespace ControlPlane
{

struct InternalAddress
{
    uint64_t m_value;

    InternalAddress( RangedValueBase const *ptr = 0 ) : m_value( reinterpret_cast<uint64_t>( ptr ) ) {}

    InternalAddress( InternalAddress const &other ) = default;
    InternalAddress &operator=( InternalAddress const &other ) = default;

    inline friend bool operator==( InternalAddress const &lhs, InternalAddress const &rhs )
    {
        return lhs.m_value == rhs.m_value;
    }

    inline friend bool operator<( InternalAddress const &lhs, InternalAddress const &rhs ) { return lhs.m_value < rhs.m_value; }

    inline friend std::ostream &operator<<( std::ostream &out, InternalAddress const &rhs )
    {
        out << rhs.m_value;
        return out;
    }
};

using SchemaInternalAddressAdaptor = SchemaAdaptor<InternalAddress>;
using InternalAddressSet = std::set<InternalAddress>;

bool getInternalAddressForIdentity( InternalAddress &address,
                                    ControlIdentity const &identity,
                                    SchemaAddress const &schema_address,
                                    Schema &schema );
}
