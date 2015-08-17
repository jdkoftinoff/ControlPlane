#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class StreamPort : public DescriptorBase
{
};

class StreamPortInput : public StreamPort
{
};

class StreamPortOutput : public StreamPort
{
};

#if 0
template <typename... T>
StreamPortInputPtr makeStreamPortInput( T &&... args )
{
    return StreamPortInputPtr( new StreamPortInput( args... ) );
}

template <typename... T>
StreamPortOutputPtr makeStreamPortOutput( T &&... args )
{
    return StreamPortOutputPtr( new StreamPortOutput( args... ) );
}
#endif
}
}
