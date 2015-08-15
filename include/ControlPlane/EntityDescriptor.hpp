#pragma once

#include "World.hpp"
#include "AvdeccSchema.hpp"
#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"
#include "Values.hpp"

namespace ControlPlane
{

class EntityDescriptor : public Descriptor
{
  public:
    EntityDescriptor( DescriptorCounts &counts,
                      std::string description,
                      DescriptorString *entity_name,
                      DescriptorString *group_name,
                      DescriptorString *firmware_version,
                      DescriptorString *serial_number,
                      EUI64 *entity_id,
                      EUI64 *entity_model_id );
    virtual ~EntityDescriptor() {}

    uint16_t getNumNames() const override { return m_names.size(); }

    DescriptorString const *getName( size_t name_index = 0 ) const override;

    DescriptorString *getName( size_t name_index = 0 ) override;

    bool setName( string val, size_t name_index = 0 ) override;

    uint64_t getAvdeccControlType() const override { return 0; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return AVDECC_CONTROL_VALUE_UTF8; }

    uint16_t getNumValues() const override { return m_items.size(); }

    uint16_t getWidth() const override { return 1; }

    uint16_t getHeight() const override { return 1; }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override;

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override;

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override;

    enum
    {
        ItemForName = 0,
        ItemForGroup,
        ItemForFirmwareVersion,
        ItemForSerialNumber,
        ItemForEntityId,
        ItemForEntityModelId
    };

  private:
    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;

    std::vector<ControlValue> m_items;
    std::vector<DescriptorString *> m_names;
};

using EntityDescriptorPtr = shared_ptr<EntityDescriptor>;

inline EntityDescriptorPtr makeEntityDescriptor( DescriptorCounts &counts,
                                                 std::string description,
                                                 DescriptorString *entity_name,
                                                 DescriptorString *group_name,
                                                 DescriptorString *firmware_version,
                                                 DescriptorString *serial_number,
                                                 EUI64 *entity_id,
                                                 EUI64 *entity_model_id )
{
    return EntityDescriptorPtr( new EntityDescriptor(
        counts, description, entity_name, group_name, firmware_version, serial_number, entity_id, entity_model_id ) );
}
}
