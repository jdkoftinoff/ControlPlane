#pragma once

#include "World.hpp"
#include "AvdeccSchema.hpp"
#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"

namespace ControlPlane
{

class StreamDescriptor : public Descriptor
{
  public:
    StreamDescriptor( DescriptorCounts &counts, string description );
    virtual ~StreamDescriptor() {}

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

  private:
    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;
    DescriptorString m_object_name;
};

using StreamDescriptorPtr = shared_ptr<StreamDescriptor>;

template <typename... T>
StreamDescriptorPtr makeStreamDescriptor( T &&... args )
{
    return StreamDescriptorPtr( new StreamDescriptor( args... ) );
}
}
