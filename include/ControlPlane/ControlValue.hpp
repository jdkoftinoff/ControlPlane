#pragma once

#include "World.hpp"
#include "RangedValue.hpp"

namespace ControlPlane
{

class ControlValue
{
  public:
    ControlValue( const string &name, RangedValueBase *ranged_value ) : m_name( name ), m_ranged_value( ranged_value ) {}

    ControlValue( RangedValueBase *ranged_value ) : m_name(), m_ranged_value( ranged_value ) {}

    ControlValue( const ControlValue &other ) : m_name( other.m_name ), m_ranged_value( other.m_ranged_value ) {}

    ControlValue() : m_name(), m_ranged_value( 0 ) {}

    ControlValue &operator=( const ControlValue &other )
    {
        m_name = other.m_name;
        m_ranged_value = other.m_ranged_value;
        return *this;
    }

    string m_name;
    RangedValueBase *m_ranged_value;
};
}
