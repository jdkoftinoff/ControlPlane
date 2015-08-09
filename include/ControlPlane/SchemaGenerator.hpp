#pragma once

#include "World.hpp"
#include "RangedValue.hpp"
#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"
#include "ControlDescriptor.hpp"
#include "MatrixDescriptor.hpp"
#include "ControlContainer.hpp"

namespace ControlPlane
{

class SchemaGenerator
{
  public:
    ControlContainerPtr m_root;
    DescriptorCounts &m_counts;

    SchemaGenerator( ControlContainerPtr root, DescriptorCounts &counts ) : m_root( root ), m_counts( counts ) {}

    virtual ControlContainerPtr generate() = 0;
};
}
