#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class InternalPort : public DescriptorBase
{
};

class InternalPortInput : public InternalPort
{
};

class InternalPortOutput : public InternalPort
{
};

#if 0
template <typename... T>
InternalPortInputPtr makeInternalPortInput( T &&... args )
{
    return InternalPortInputPtr( new InternalPortInput( args... ) );
}

template <typename... T>
InternalPortOutputPtr makeInternalPortOutput( T &&... args )
{
    return InternalPortOutputPtr( new InternalPortOutput( args... ) );
}
#endif
}
}
