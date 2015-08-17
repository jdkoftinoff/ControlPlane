#pragma once

#include "../World.hpp"
#include "../ControlValue.hpp"
#include "../ControlIdentity.hpp"
#include "../Values.hpp"
#include "../ControlIdentityComparator.hpp"
#include "../FixedBuffer.hpp"
#include "DescriptorCounts.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class DescriptorBase
{
  protected:
    virtual void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) = 0;

  public:
    virtual ~DescriptorBase() {}
    virtual uint16_t getNumNames() const = 0;
    virtual DescriptorString const *getName( size_t name_index = 0 ) const = 0;
    virtual DescriptorString *getName( size_t name_index = 0 ) = 0;
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
    virtual void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) = 0;
    virtual void storeToPDU( FixedBuffer &pdu ) const = 0;
    virtual void collectOwnedDescriptors( DescriptorCounts &counts );

    ControlIdentity getControlIdentity() const;

    ControlIdentity getControlIdentityForItem( uint16_t item_num ) const;
    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t h_pos ) const;

    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t h_pos, uint16_t w_pos ) const;

    ControlIdentity getControlIdentityForName( uint16_t name_num = 0 ) const;

    void addChildDescriptor( std::shared_ptr<DescriptorBase> d );

    size_t getNumChildDescriptorsForType( uint16_t type ) const;

    template <typename DescriptorT>
    std::vector<std::shared_ptr<DescriptorT> > *getChildDescriptorsFor()
    {
        std::vector<std::shared_ptr<DescriptorT> > *getChildDescriptorsForType( DescriptorT::descriptor_type );
    }

    std::vector<std::shared_ptr<DescriptorBase> > *getChildDescriptorsForType( uint16_t type );
    const std::vector<std::shared_ptr<DescriptorBase> > *getChildDescriptorsForType( uint16_t type ) const;

    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > &getChildDescriptors() { return m_child_descriptors; }

    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > const &getChildDescriptors() const
    {
        return m_child_descriptors;
    }

  private:
    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > m_child_descriptors;
};

using DescriptorPtr = shared_ptr<DescriptorBase>;
using DescriptorAddressMap = map<SchemaAddressElement, DescriptorPtr>;
using DescriptorAvdeccMap = map<ControlIdentity, DescriptorPtr>;

using CpIndex = vector<int>;
using DescriptorCpIndexMap = map<CpIndex, DescriptorPtr>;
}
}
