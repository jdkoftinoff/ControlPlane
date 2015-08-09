#include "ControlPlane/World.hpp"
#include "ControlPlane/StreamDescriptor.hpp"

namespace ControlPlane
{

const DescriptorObjectName *StreamDescriptor::getName( size_t name_index ) const
{
    DescriptorObjectName const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorObjectName *StreamDescriptor::getName( size_t name_index )
{
    DescriptorObjectName *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool StreamDescriptor::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}
