#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"
#include "MatrixSignal.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Matrix : public DescriptorBase
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_MATRIX;

    Matrix( uint64_t avdecc_control_type, string description, uint16_t avdecc_control_value_type )
        : DescriptorBase( description, descriptor_type )
        , m_avdecc_control_type( avdecc_control_type )
        , m_avdecc_control_value_type( avdecc_control_value_type )
        , m_current_matrix_signal_descriptor( makeMatrixSignal() )
    {
        addChildDescriptor( m_current_matrix_signal_descriptor );
    }

    ~Matrix();

    void addRow( DescriptorPtr source_signal, uint16_t source_signal_output )
    {
        m_control_point_values.emplace_back();
        if ( !m_current_matrix_signal_descriptor->addSignal( source_signal, source_signal_output ) )
        {
            m_current_matrix_signal_descriptor = makeMatrixSignal();
            addChildDescriptor( m_current_matrix_signal_descriptor );
        }
    }

    void addColumn() { m_control_point_values.back().emplace_back(); }

    void addValue( ControlValue v ) { m_control_point_values.back().back().push_back( v ); }

    uint64_t getAvdeccControlType() const override { return m_avdecc_control_type; }

    uint16_t getAvdeccControlValueType() const override { return m_avdecc_control_value_type; }

    virtual uint16_t getNumValues() const override { return (uint16_t)m_control_point_values.back().back().size(); }

    virtual uint16_t getWidth() const override { return (uint16_t)m_control_point_values.back().size(); }

    virtual uint16_t getHeight() const override { return (uint16_t)m_control_point_values.size(); }

    ControlValue &getValue( size_t item_num, size_t w, size_t h ) override { return m_control_point_values[h][w][item_num]; }

    const ControlValue &getValue( size_t item_num, size_t w, size_t h ) const override
    {
        return m_control_point_values[h][w][item_num];
    }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}

    void storeToPDU( FixedBuffer &pdu ) const override;

    uint64_t m_avdecc_control_type;
    uint16_t m_avdecc_control_value_type;
    vector<vector<vector<ControlValue> > > m_control_point_values;
    MatrixSignalPtr m_current_matrix_signal_descriptor;
};

template <typename... T>
MatrixPtr makeMatrix( T &&... args )
{
    return MatrixPtr( new Matrix( args... ) );
}
}
}
