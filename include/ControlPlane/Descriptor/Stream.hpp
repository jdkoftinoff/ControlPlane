#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{
class Stream : public DescriptorBase
{
  protected:
    void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) override
    {
        m_avdecc_descriptor_index = new_descriptor_index;
    }

  public:
    Stream( DescriptorCounts &counts, string description );
    virtual ~Stream() {}

    uint16_t getNumNames() const override { return 1; }

    DescriptorString const *getName( size_t name_index = 0 ) const override;

    DescriptorString *getName( size_t name_index = 0 ) override;

    bool setName( string val, size_t name_index = 0 ) override;

    uint64_t getAvdeccControlType() const override { return 0; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return 0xffff; }

    virtual uint16_t getNumValues() const override { return 0; }

    virtual uint16_t getWidth() const override { return 0; }

    virtual uint16_t getHeight() const override { return 0; }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override { throw std::runtime_error( "no value" ); }

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override
    {
        throw std::runtime_error( "no value" );
    }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override;

    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;
    DescriptorString m_object_name;
};

class StreamInput : public Stream
{
  public:
};

class StreamOutput : public Stream
{
  public:
};

template <typename... T>
StreamPtr makeStream( T &&... args )
{
    return StreamPtr( new Stream( args... ) );
}
}
}
