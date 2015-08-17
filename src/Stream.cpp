#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Stream.hpp"

namespace ControlPlane
{
namespace Descriptor
{
const DescriptorString *Stream::getName( size_t name_index ) const
{
    DescriptorString const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorString *Stream::getName( size_t name_index )
{
    DescriptorString *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool Stream::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}
}
