#pragma once

#include "../World.hpp"
#include "../AvdeccSchema.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class DescriptorCounts
{
  public:
    uint16_t getCountForDescriptorTypeAndIncrement( uint16_t descriptor_type )
    {
        return m_descriptor_index_for_type[descriptor_type]++;
    }

    uint16_t getCountForDescriptorType( uint16_t descriptor_type ) const
    {
        return m_descriptor_index_for_type[descriptor_type];
    }

  private:
    std::array<uint16_t, AVDECC_NUM_DESCRIPTOR_TYPES> m_descriptor_index_for_type;
};
}
}
