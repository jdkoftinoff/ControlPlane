#pragma once

#include "World.hpp"
#include "RangedValue.hpp"
#include "Descriptors.hpp"
#include "ControlContainer.hpp"

namespace ControlPlane
{

using Descriptor::DescriptorCounts;

class SchemaGenerator
{
  public:
    ControlContainerPtr m_root;
    DescriptorCounts &m_counts;

    SchemaGenerator( ControlContainerPtr root, DescriptorCounts &counts ) : m_root( root ), m_counts( counts ) {}

    virtual ControlContainerPtr generate() = 0;
};
}
