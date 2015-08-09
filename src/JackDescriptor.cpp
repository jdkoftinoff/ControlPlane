#include "ControlPlane/World.hpp"
#include "ControlPlane/JackDescriptor.hpp"

namespace ControlPlane
{

const DescriptorObjectName *JackDescriptor::getName( size_t name_index ) const
{
    DescriptorObjectName const *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

DescriptorObjectName *JackDescriptor::getName( size_t name_index )
{
    DescriptorObjectName *r = nullptr;
    if ( name_index == 0 )
    {
        r = &m_object_name;
    }
    return r;
}

bool JackDescriptor::setName( std::string val, size_t name_index )
{
    bool r = false;
    if ( name_index == 0 )
    {
        r = m_object_name.setValue( val );
    }
    return r;
}
}
