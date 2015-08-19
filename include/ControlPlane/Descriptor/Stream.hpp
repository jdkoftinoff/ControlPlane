#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{
class Stream : public DescriptorBase
{
  public:
    Stream( string description, uint16_t descriptor_type ) : DescriptorBase( description, descriptor_type ) {}

    uint64_t getAvdeccControlType() const override { return 0; }

    uint16_t getAvdeccControlValueType() const override { return 0xffff; }

    virtual uint16_t getNumValues() const override { return 0; }

    virtual uint16_t getWidth() const override { return 0; }

    virtual uint16_t getHeight() const override { return 0; }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override;
};

class StreamInput : public Stream
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_STREAM_INPUT;
    StreamInput( string description ) : Stream( description, descriptor_type ) {}
};

class StreamOutput : public Stream
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_STREAM_OUTPUT;
    StreamOutput( string description ) : Stream( description, descriptor_type ) {}
};

template <typename... T>
StreamPtr makeStream( T &&... args )
{
    return StreamPtr( new Stream( args... ) );
}
}
}
