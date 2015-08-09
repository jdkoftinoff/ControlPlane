#pragma once

#include "World.hpp"
#include "ControlValue.hpp"
#include "ControlIdentity.hpp"

namespace ControlPlane
{

using DescriptorObjectName = RangedValue<UnitsCode::Unitless, 0, 0, 0, 0, 0, ControlPlane::AvdeccNameString, std::string>;

class Descriptor
{
  public:
    virtual ~Descriptor() {}
    virtual uint16_t getNumNames() const = 0;
    virtual DescriptorObjectName const *getName( size_t name_index = 0 ) const = 0;
    virtual DescriptorObjectName *getName( size_t name_index = 0 ) = 0;
    virtual bool setName( string val, size_t name_index = 0 ) = 0;
    virtual uint64_t getAvdeccControlType() const = 0;
    virtual uint16_t getAvdeccDescriptorType() const = 0;
    virtual uint16_t getAvdeccDescriptorIndex() const = 0;
    virtual string getDescription() const = 0;
    virtual uint16_t getAvdeccControlValueType() const = 0;
    virtual uint16_t getNumValues() const = 0;
    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
    virtual ControlValue &getValue( size_t item_num, size_t w = 0, size_t h = 0 ) = 0;
    virtual const ControlValue &getValue( size_t item_num, size_t w = 0, size_t h = 0 ) const = 0;

    ControlIdentity getControlIdentity() const
    {
        ControlIdentity r = ControlIdentity{getAvdeccDescriptorType(), getAvdeccDescriptorIndex()};
        return r;
    }

    ControlIdentity getControlIdentityForItem( uint16_t item_num ) const
    {
        ControlIdentity r = ControlIdentity(
            getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionItemLevel, item_num );
        return r;
    }
    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t w_pos ) const
    {
        ControlIdentity r = ControlIdentity(
            getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionWPosLevel, item_num, w_pos );
        return r;
    }

    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t w_pos, uint16_t h_pos ) const
    {
        ControlIdentity r = ControlIdentity(
            getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionHPosLevel, item_num, w_pos, h_pos );
        return r;
    }

    ControlIdentity getControlIdentityForName( uint16_t name_num = 0 ) const
    {
        ControlIdentity r
            = ControlIdentity( getAvdeccDescriptorType(), getAvdeccDescriptorIndex(), ControlIdentity::SectionName, name_num );
        return r;
    }
};

using DescriptorPtr = shared_ptr<Descriptor>;
using DescriptorAddressMap = map<SchemaAddressElement, DescriptorPtr>;
using DescriptorAvdeccMap = map<ControlIdentity, DescriptorPtr>;

using CpIndex = vector<int>;
using DescriptorCpIndexMap = map<CpIndex, DescriptorPtr>;
}
