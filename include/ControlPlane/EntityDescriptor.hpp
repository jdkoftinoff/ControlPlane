#pragma once

#include "World.hpp"
#include "AvdeccSchema.hpp"
#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"

namespace ControlPlane
{

class EntityDescriptor : public Descriptor
{
  public:
    EntityDescriptor( DescriptorCounts &counts, string description );
    virtual ~EntityDescriptor() {}

    uint16_t getNumNames() const override { return m_names.size(); }

    DescriptorObjectName const *getName( size_t name_index = 0 ) const override;

    DescriptorObjectName *getName( size_t name_index = 0 ) override;

    bool setName( string val, size_t name_index = 0 ) override;

    uint64_t getAvdeccControlType() const override { return 0; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return AVDECC_CONTROL_VALUE_UTF8; }

    uint16_t getNumValues() const override { return m_items.size(); }

    uint16_t getWidth() const override { return 0; }

    uint16_t getHeight() const override { return 0; }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override;

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override;

  private:
    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;
    DescriptorObjectName m_firmware_version;
    DescriptorObjectName m_serial_number;

    std::vector<ControlValue> m_items;

    std::array<DescriptorObjectName, 2> m_names;
};

template <typename... T>
DescriptorPtr makeEntityDescriptor( T &&... args )
{
    return DescriptorPtr( new EntityDescriptor( args... ) );
}
}
