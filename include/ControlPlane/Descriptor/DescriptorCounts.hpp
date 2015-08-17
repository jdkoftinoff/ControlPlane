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
    DescriptorCounts() : m_num_configs( 0 ) { addConfiguration(); }

    uint16_t addConfiguration()
    {
        m_descriptor_index_for_type.push_back( std::array<uint16_t, AVDECC_NUM_DESCRIPTOR_TYPES>() );
        m_num_configs++;
        for ( auto &i : m_descriptor_index_for_type[m_num_configs] )
        {
            i = 0;
        }

        return m_num_configs - 1;
    }

    uint16_t getCountForDescriptorTypeAndIncrement( uint16_t descriptor_type )
    {
        return m_descriptor_index_for_type[m_num_configs - 1][descriptor_type]++;
    }

    uint16_t getCountForDescriptorType( uint16_t descriptor_type ) const
    {
        return m_descriptor_index_for_type[m_num_configs - 1][descriptor_type];
    }

  private:
    int m_num_configs;
    std::vector<std::array<uint16_t, AVDECC_NUM_DESCRIPTOR_TYPES> > m_descriptor_index_for_type;
};
}
}
