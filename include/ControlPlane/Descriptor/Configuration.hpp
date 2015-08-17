#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Configuration : public DescriptorBase
{
  protected:
    void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) override
    {
        m_avdecc_descriptor_index = new_descriptor_index;
    }

  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_CONFIGURATION;

    Configuration( string description )
        : m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_CONFIGURATION )
        , m_avdecc_descriptor_index( 0 )
        , m_description( description )
        , m_object_name()
    {
    }
    virtual ~Configuration() {}

    uint16_t getNumNames() const override { return 1; }

    DescriptorString const *getName( size_t name_index = 0 ) const override
    {
        DescriptorString const *r = 0;
        if ( name_index == 0 )
        {
            r = &m_object_name;
        }
        return r;
    }

    DescriptorString *getName( size_t name_index = 0 ) override
    {
        DescriptorString *r = 0;
        if ( name_index == 0 )
        {
            r = &m_object_name;
        }
        return r;
    }

    bool setName( string val, size_t name_index = 0 ) override
    {
        bool changed = false;
        if ( name_index == 0 )
        {
            changed = m_object_name.setValue( val );
        }
        return changed;
    }

    uint64_t getAvdeccControlType() const override { return 0; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return 0xffff; }

    uint16_t getNumValues() const override { return 0; }

    uint16_t getWidth() const override { return 0; }

    uint16_t getHeight() const override { return 0; }

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

template <typename... T>
ConfigurationPtr makeConfiguration( T &&... args )
{
    return ConfigurationPtr( new Configuration( args... ) );
}
}
}
