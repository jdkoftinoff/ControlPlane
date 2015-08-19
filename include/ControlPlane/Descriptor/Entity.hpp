#pragma once

#include "../World.hpp"
#include "Descriptor.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Entity : public DescriptorBase
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_ENTITY;

    Entity( std::string description,
            DescriptorString *entity_name,
            DescriptorString *group_name,
            DescriptorString *firmware_version,
            DescriptorString *serial_number,
            RangedValueEUI64 *entity_id,
            RangedValueEUI64 *entity_model_id );

    uint16_t getNumValues() const override { return (uint16_t)m_items.size(); }

    uint16_t getWidth() const override { return 1; }

    uint16_t getHeight() const override { return 1; }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override;

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override;

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override;

    void storeToPDU( FixedBuffer &pdu ) const override;

    enum
    {
        ItemForName = 0,
        ItemForGroup,
        ItemForFirmwareVersion,
        ItemForSerialNumber,
        ItemForEntityId,
        ItemForEntityModelId
    };

    std::vector<ControlValue> m_items;
};

inline EntityPtr makeEntity( std::string description,
                             DescriptorString *entity_name,
                             DescriptorString *group_name,
                             DescriptorString *firmware_version,
                             DescriptorString *serial_number,
                             RangedValueEUI64 *entity_id,
                             RangedValueEUI64 *entity_model_id )
{
    return EntityPtr(
        new Entity( description, entity_name, group_name, firmware_version, serial_number, entity_id, entity_model_id ) );
}
}
}
