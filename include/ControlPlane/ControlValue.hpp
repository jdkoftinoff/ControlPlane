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

    string m_name;
    RangedValueBase *m_ranged_value;
};
}
