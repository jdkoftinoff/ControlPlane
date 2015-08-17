#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Control : public DescriptorBase
{
  protected:
    void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) override
    {
        m_avdecc_descriptor_index = new_descriptor_index;
    }

  private:
    template <typename T>
    static void push_back_multi( vector<T> &vec, T first )
    {
        vec.push_back( first );
    }

    template <typename T, typename... ArgT>
    static void push_back_multi( vector<T> &vec, T first, ArgT &&... args )
    {
        vec.push_back( first );
        push_back_multi( vec, args... );
    }

  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_CONTROL;

    Control( uint64_t avdecc_control_type, string description, uint16_t avdecc_control_value_type, ControlValue first );

    template <typename... ArgT>
    Control( uint64_t avdecc_control_type,
             string description,
             uint16_t avdecc_control_value_type,
             ControlValue first,
             ArgT &&... args )
        : m_avdecc_control_type( avdecc_control_type )
        , m_avdecc_descriptor_type( AVDECC_DESCRIPTOR_CONTROL )
        , m_description( description )
        , m_avdecc_control_value_type( avdecc_control_value_type )
    {
        push_back_multi( m_control_point_values, first, args... );
    }

    virtual ~Control() {}

    uint16_t getNumNames() const override { return 1; }

    DescriptorString const *getName( size_t name_index = 0 ) const override;

    DescriptorString *getName( size_t name_index = 0 ) override;

    bool setName( string val, size_t name_index = 0 ) override;

    uint64_t getAvdeccControlType() const override { return m_avdecc_control_type; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return m_avdecc_control_value_type; }

    virtual uint16_t getNumValues() const override { return (uint16_t)m_control_point_values.size(); }

    virtual uint16_t getWidth() const override { return 1; }

    virtual uint16_t getHeight() const override { return 1; }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override { return m_control_point_values[item_num]; }

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override
    {
        return m_control_point_values[item_num];
    }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override { /* TODO */}

    uint64_t m_avdecc_control_type;
    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;
    uint16_t m_avdecc_control_value_type;
    DescriptorString m_object_name;
    vector<ControlValue> m_control_point_values;
};

template <typename... T>
ControlPtr makeControl( T &&... args )
{
    return ControlPtr( new Control( args... ) );
}
}
}
