#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Matrix : public DescriptorBase
{
  protected:
    void setAvdeccDescriptorIndex( uint16_t new_descriptor_index ) override
    {
        m_avdecc_descriptor_index = new_descriptor_index;
    }

  public:
    Matrix( DescriptorCounts &counts, uint64_t avdecc_control_type, string description, uint16_t avdecc_control_value_type );

    virtual ~Matrix() {}

    uint16_t getNumNames() const override { return 1; }

    DescriptorString const *getName( size_t name_index = 0 ) const override;

    DescriptorString *getName( size_t name_index = 0 ) override;

    bool setName( string val, size_t name_index = 0 ) override;

    void addRow() { m_control_point_values.emplace_back(); }

    void addColumn() { m_control_point_values.back().emplace_back(); }

    void addValue( ControlValue v ) { m_control_point_values.back().back().push_back( v ); }

    uint64_t getAvdeccControlType() const override { return m_avdecc_control_type; }

    uint16_t getAvdeccDescriptorType() const override { return m_avdecc_descriptor_type; }

    uint16_t getAvdeccDescriptorIndex() const override { return m_avdecc_descriptor_index; }

    string getDescription() const override { return m_description; }

    uint16_t getAvdeccControlValueType() const override { return m_avdecc_control_value_type; }

    virtual uint16_t getNumValues() const override { return m_control_point_values.back().back().size(); }

    virtual uint16_t getWidth() const override { return m_control_point_values.back().size(); }

    virtual uint16_t getHeight() const override { return m_control_point_values.size(); }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override { return m_control_point_values[h][w][item_num]; }

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override
    {
        return m_control_point_values[h][w][item_num];
    }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override;

    void collectOwnedDescriptors( DescriptorCounts &counts ) override;

    uint64_t m_avdecc_control_type;
    uint16_t m_avdecc_descriptor_type;
    uint16_t m_avdecc_descriptor_index;
    string m_description;
    uint16_t m_avdecc_control_value_type;
    DescriptorString m_object_name;
    vector<vector<vector<ControlValue> > > m_control_point_values;
    vector<MatrixSignalPtr> m_matrix_signals;
};

template <typename... T>
MatrixPtr makeMatrix( T &&... args )
{
    return MatrixPtr( new Matrix( args... ) );
}
}
}
