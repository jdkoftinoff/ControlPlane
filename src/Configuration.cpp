#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Configuration.hpp"
#include "ControlPlane/Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

template <typename T>
void collectOwnedDescriptorsInVector( DescriptorCounts &counts, std::vector<std::shared_ptr<T> > &items )
{
    for ( auto &i : items )
    {
        i->collectOwnedDescriptors( counts );
    }
}

template <typename T>
void putDescriptorCounts( FixedBuffer &pdu, const std::vector<std::shared_ptr<T> > &items )
{
    if ( items.size() > 0 )
    {
        pdu.putDoublet( items[0]->getAvdeccDescriptorType() );
        pdu.putDoublet( (uint16_t)items.size() );
    }
}

void Configuration::storeToPDU( FixedBuffer &pdu ) const
{
    pdu.putDoublet( m_avdecc_descriptor_type );
    pdu.putDoublet( m_avdecc_descriptor_index );
    pdu.putAvdeccString( m_object_name.getValue() );
    pdu.putDoublet( 0xffff );

    size_t descriptor_counts_count = 0;

    descriptor_counts_count += m_audio_unit.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_stream_input.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_audio_unit.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_stream_input.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_stream_output.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_jack_input.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_jack_output.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_avb_interface.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_clock_source.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_control.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_signal_selector.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_mixer.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_matrix.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_locale.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_memory_object.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_clock_domain.size() > 0 ? 1 : 0;
    descriptor_counts_count += m_control_block.size() > 0 ? 1 : 0;

    pdu.putDoublet( (uint16_t)descriptor_counts_count );
    pdu.putDoublet( 74 );

    putDescriptorCounts( pdu, m_audio_unit );
    putDescriptorCounts( pdu, m_stream_input );
    putDescriptorCounts( pdu, m_audio_unit );
    putDescriptorCounts( pdu, m_stream_input );
    putDescriptorCounts( pdu, m_stream_output );
    putDescriptorCounts( pdu, m_jack_input );
    putDescriptorCounts( pdu, m_jack_output );
    putDescriptorCounts( pdu, m_avb_interface );
    putDescriptorCounts( pdu, m_clock_source );
    putDescriptorCounts( pdu, m_control );
    putDescriptorCounts( pdu, m_signal_selector );
    putDescriptorCounts( pdu, m_mixer );
    putDescriptorCounts( pdu, m_matrix );
    putDescriptorCounts( pdu, m_locale );
    putDescriptorCounts( pdu, m_memory_object );
    putDescriptorCounts( pdu, m_clock_domain );
    putDescriptorCounts( pdu, m_control_block );
}

void Configuration::collectOwnedDescriptors( DescriptorCounts &counts )
{
    DescriptorBase::collectOwnedDescriptors( counts );

    collectOwnedDescriptorsInVector( counts, m_audio_unit );
    collectOwnedDescriptorsInVector( counts, m_stream_input );

    collectOwnedDescriptorsInVector( counts, m_audio_unit );
    collectOwnedDescriptorsInVector( counts, m_stream_input );
    collectOwnedDescriptorsInVector( counts, m_stream_output );
    collectOwnedDescriptorsInVector( counts, m_jack_input );
    collectOwnedDescriptorsInVector( counts, m_jack_output );
    collectOwnedDescriptorsInVector( counts, m_avb_interface );
    collectOwnedDescriptorsInVector( counts, m_clock_source );
    collectOwnedDescriptorsInVector( counts, m_control );
    collectOwnedDescriptorsInVector( counts, m_signal_selector );
    collectOwnedDescriptorsInVector( counts, m_mixer );
    collectOwnedDescriptorsInVector( counts, m_matrix );
    collectOwnedDescriptorsInVector( counts, m_locale );
    collectOwnedDescriptorsInVector( counts, m_memory_object );
    collectOwnedDescriptorsInVector( counts, m_clock_domain );
    collectOwnedDescriptorsInVector( counts, m_control_block );
}
}
}
