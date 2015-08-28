#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class MatrixSignal : public DescriptorBase
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_ENTITY;
    using SignalSource = std::tuple<std::weak_ptr<DescriptorBase>, uint16_t>;

    MatrixSignal() : DescriptorBase( "", descriptor_type ) {}

    bool addSignal( std::weak_ptr<DescriptorBase> descriptor, uint16_t signal_output )
    {
        bool r = false;
        if ( m_signals.size() <= 83 )
        {
            m_signals.push_back( SignalSource{descriptor, signal_output} );
            r = true;
        }
        return r;
    }

    std::vector<SignalSource> const &getSignals() const { return m_signals; }

    size_t getNumSignals() const { return m_signals.size(); }

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override { /* DO NOTHING */}

    void storeToPDU( FixedBuffer &pdu ) const override { /* TODO */}

  private:
    std::vector<SignalSource> m_signals;
};

inline MatrixSignalPtr makeMatrixSignal() { return MatrixSignalPtr( new MatrixSignal() ); }
}
}
