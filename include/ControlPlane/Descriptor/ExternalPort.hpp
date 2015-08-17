#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class ExternalPort : public DescriptorBase
{
};

class ExternalPortInput : public ExternalPort
{
};

class ExternalPortOutput : public ExternalPort
{
};

#if 0
template <typename... T>
ExternalPortInputPtr makeExternalPortInput( T &&... args )
{
    return ExternalPortInputPtr( new ExternalPortInput( args... ) );
}

template <typename... T>
ExternalPortOutputPtr makeExternalPortOutput( T &&... args )
{
    return ExternalPortOutputPtr( new ExternalPortOutput( args... ) );
}
#endif
}
}
