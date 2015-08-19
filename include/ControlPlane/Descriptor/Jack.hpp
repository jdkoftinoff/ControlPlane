#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Jack : public DescriptorBase
{
  public:
    Jack( string description, uint16_t descriptor_type ) : DescriptorBase( description, descriptor_type ) {}

    uint16_t getAvdeccControlValueType() const override { return 0xffff; }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override;
};

class JackInput : public Jack
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_JACK_INPUT;
    JackInput( string description ) : Jack( description, descriptor_type ) {}
};

class JackOutput : public Jack
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_JACK_OUTPUT;
    JackOutput( string description ) : Jack( description, descriptor_type ) {}
};

template <typename... T>
JackInputPtr makeJackInput( T &&... args )
{
    return JackInputPtr( new JackInput( args... ) );
}

template <typename... T>
JackOutputPtr makeJackOutput( T &&... args )
{
    return JackOutputPtr( new JackOutput( args... ) );
}
}
}
