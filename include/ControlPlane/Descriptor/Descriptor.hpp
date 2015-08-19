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
    std::string m_description;
    uint16_t m_descriptor_type;
    uint16_t m_descriptor_index;
    DescriptorString m_object_name;
    std::vector<DescriptorString *> m_names;
    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > m_child_descriptors;

  protected:
    DescriptorBase( std::string description, uint16_t descriptor_type, uint16_t descriptor_index = 0 )
        : m_description( description ), m_descriptor_type( descriptor_type ), m_descriptor_index( descriptor_index )
    {
        m_names.push_back( &m_object_name );
    }

    void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) { m_descriptor_index = new_descriptor_index; }

    void addName( DescriptorString *n ) { m_names.push_back( n ); }

  public:
    uint16_t getAvdeccDescriptorType() const { return m_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const { return m_descriptor_index; }

    uint16_t getNumNames() const { return (uint16_t)m_names.size(); }

    DescriptorString const *getName( size_t name_index = 0 ) const { return m_names[name_index]; }

    DescriptorString *getName( size_t name_index = 0 ) { return m_names[name_index]; }

    bool setName( string val, size_t name_index = 0 ) { return m_names[name_index]->setValue( val ); }

    string const &getDescription() const { return m_description; }

    virtual ~DescriptorBase() {}

    virtual void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) = 0;
    virtual void collectOwnedDescriptors( DescriptorCounts &counts );
    virtual void storeToPDU( FixedBuffer &pdu ) const = 0;

    virtual uint64_t getAvdeccControlType() const { throw std::runtime_error( "no getAvdeccControlType for descriptor" ); }

    virtual uint16_t getAvdeccControlValueType() const
    {
        throw std::runtime_error( "no getAvdeccControlValueType for descriptor" );
    }

    virtual uint16_t getNumValues() const { return 0; }

    virtual uint16_t getWidth() const { return 0; }

    virtual uint16_t getHeight() const { return 0; }

    virtual ControlValue &getValue( size_t item_num, size_t w = 0, size_t h = 0 )
    {
        throw std::runtime_error( "no getValue for descriptor" );
    }

    virtual const ControlValue &getValue( size_t item_num, size_t w = 0, size_t h = 0 ) const
    {
        throw std::runtime_error( "no getValue for descriptor" );
    }

    ControlIdentity getControlIdentity() const { return ControlIdentity( m_descriptor_type, m_descriptor_index ); }

    ControlIdentity getControlIdentityForItem( uint16_t item_num ) const
    {
        return ControlIdentity( m_descriptor_type, m_descriptor_index, ControlIdentity::SectionDescriptorLevel, item_num );
    }

    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t h_pos ) const
    {
        return ControlIdentity( m_descriptor_type, m_descriptor_index, ControlIdentity::SectionHPosLevel, item_num, h_pos );
    }

    ControlIdentity getControlIdentityForItem( uint16_t item_num, uint16_t h_pos, uint16_t w_pos ) const
    {
        return ControlIdentity(
            m_descriptor_type, m_descriptor_index, ControlIdentity::SectionWPosLevel, item_num, h_pos, w_pos );
    }

    ControlIdentity getControlIdentityForName( uint16_t name_num = 0 ) const
    {
        return ControlIdentity( m_descriptor_type, m_descriptor_index, ControlIdentity::SectionName, name_num );
    }

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
};

using DescriptorPtr = shared_ptr<DescriptorBase>;
using DescriptorAddressMap = map<SchemaAddressElement, DescriptorPtr>;
using DescriptorAvdeccMap = map<ControlIdentity, DescriptorPtr>;
}
}
