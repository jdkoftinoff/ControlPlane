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
class ControlContainer;
using ControlContainerPtr = std::shared_ptr<ControlContainer>;
namespace Descriptor
{
class DescriptorBase;
using DescriptorPropertyMap = std::map<std::string, ControlValue>;
using DescriptorProperties = std::vector<ControlValue>;
using DescriptorNames = std::vector<ControlValue>;
using DescriptorPtr = shared_ptr<DescriptorBase>;
using DescriptorAddressMap = map<SchemaAddressElement, std::pair<DescriptorPtr, ControlIdentity> >;
using DescriptorAvdeccMap = map<ControlIdentity, DescriptorPtr>;
using DescriptorVector = std::vector<DescriptorPtr>;
using DescriptorMap = std::map<DescriptorType, DescriptorVector>;

class DescriptorBase : public std::enable_shared_from_this<DescriptorBase>
{
    std::string m_description;
    DescriptorType m_descriptor_type;
    DescriptorIndex m_descriptor_index;
    DescriptorMap m_child_descriptor_map;
    DescriptorPropertyMap m_property_map;
    DescriptorProperties m_properties;
    DescriptorNames m_names;

  protected:
    DescriptorBase( std::string description, uint16_t descriptor_type, uint16_t descriptor_index = 0 )
        : m_description( description ), m_descriptor_type( descriptor_type ), m_descriptor_index( descriptor_index )
    {
    }

    void setAvdeccDescriptorIndex( DescriptorIndex new_descriptor_index ) { m_descriptor_index = new_descriptor_index; }

    void addName( std::string name, DescriptorString *n )
    {
        addProperty( name, n );
        m_names.push_back( ControlValue( name, n ) );
    }

    void addProperty( std::string name, RangedValueBase *v )
    {
        m_property_map[name] = ControlValue( name, v );
        m_properties.push_back( ControlValue( name, v ) );
    }

  public:
    virtual ~DescriptorBase() {}

    string const &getDescription() const { return m_description; }

    DescriptorType getAvdeccDescriptorType() const { return m_descriptor_type; }

    DescriptorIndex getAvdeccDescriptorIndex() const { return m_descriptor_index; }

    virtual DescriptorString *getObjectName() { return 0; }

    virtual const DescriptorString *getObjectName() const { return 0; }

    virtual bool setObjectName( string value ) { return false; }

    uint16_t getNumProperties() const { return (uint16_t)m_properties.size(); }

    ControlValue getProperty( uint16_t item ) const { return m_properties.at( item ); }

    ControlValue getProperty( string name ) const { return m_property_map.at( name ); }

    uint16_t getNumNames() const { return (uint16_t)m_names.size(); }

    ControlValue getName( std::string name ) const { return m_property_map.at( name ); }

    ControlValue getName( uint16_t num ) const { return m_names[num]; }

    bool setName( uint16_t num, std::string value ) { return getName( num ).m_ranged_value->setUnencodedValueString( value ); }

    virtual void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) = 0;
    virtual void collectOwnedDescriptors( DescriptorCounts &counts, ControlContainerPtr &top );
    virtual void storeToPDU( FixedBuffer &pdu ) const = 0;

    virtual ControlType getAvdeccControlType() const { return 0; }

    virtual ControlValueType getAvdeccControlValueType() const { return 0; }

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

    ControlIdentity getControlIdentityForProperty( std::string const &item_name ) const
    {
        ControlIdentity r = getControlIdentity();
        for ( uint16_t i = 0; i < m_properties.size(); ++i )
        {
            if ( m_properties[i].m_name == item_name )
            {
                r.m_section = ControlIdentity::Section::SectionProperty;
                r.m_item = i;
                break;
            }
        }
        return r;
    }

    ControlIdentity getControlIdentityForProperty( uint16_t item_num ) const
    {
        return ControlIdentity( m_descriptor_type, m_descriptor_index, ControlIdentity::Section::SectionProperty, item_num );
    }

    std::string getPropertyName( uint16_t item_num ) const { return getProperty( item_num ).m_name; }

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

    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > &getChildDescriptors() { return m_child_descriptor_map; }

    std::map<uint16_t, std::vector<std::shared_ptr<DescriptorBase> > > const &getChildDescriptors() const
    {
        return m_child_descriptor_map;
    }

  private:
};
}
}
