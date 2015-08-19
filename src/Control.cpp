#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Control.hpp"

namespace ControlPlane
{
namespace Descriptor
{

Control::Control( uint64_t avdecc_control_type,
                  std::string description,
                  uint16_t avdecc_control_value_type,
                  ControlValue first )
    : DescriptorBase( description, descriptor_type )
    , m_avdecc_control_type( avdecc_control_type )
    , m_avdecc_control_value_type( avdecc_control_value_type )
{
    push_back_multi( m_control_point_values, first );
}
}
}
