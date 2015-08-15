#include "ControlPlane/World.hpp"
#include "ControlPlane/TextIOWithStream.hpp"
#include "ControlPlane/SchemaGenerator.hpp"
#include "ControlPlane/Values.hpp"

using namespace ControlPlane;
using namespace ControlPlane::Text;
using namespace ControlPlane::Util;

/** InputProcessing
 */
struct InputProcessing
{
    Mute m_mute;
    Gain m_gain;
};


/** OutputProcessing
 */
struct OutputProcessing
{
    Mute m_mute;
    Gain m_gain;
};


struct ProcessingSnapshot
{
    std::array<InputProcessing, 8> m_input;
    std::array<OutputProcessing, 24> m_output;
};


class ExampleSchemaGenerator : public SchemaGenerator
{
    ProcessingSnapshot *m_processing_settings;

public:

    ExampleSchemaGenerator( ControlContainerPtr root,
                            DescriptorCounts &counts,
                            ProcessingSnapshot *processing_settings )
        : SchemaGenerator(root,counts)
        , m_processing_settings( processing_settings )
    {
    }

    ControlContainerPtr generate() override
    {
        ControlContainerPtr schema_input = m_root->addItem( "input" );

        for ( int chan = 0; chan < m_processing_settings->m_input.size(); ++chan )
        {
            addInputChannel( schema_input, chan, &m_processing_settings->m_input[chan] );
        }

        ControlContainerPtr schema_output = m_root->addItem( "output" );

        for ( int chan = 0; chan < m_processing_settings->m_output.size(); ++chan )
        {
            addOutputChannel( schema_output, chan, &m_processing_settings->m_output[chan] );
        }
        return m_root;
    }

    ControlContainerPtr addInputChannel( ControlContainerPtr root, int chan, InputProcessing *v )
    {
        string chan_name = formstring( chan + 1 );
        string description = formstring( "Input ", chan_name );

        ControlContainerPtr schema_chan = root->addItem( chan_name );

        schema_chan->addItem( "gain", makeGainControlDescriptor( formstring( description, " Gain" ), &v->m_gain ) );

        schema_chan->addItem( "mute", makeMuteControlDescriptor( formstring( description, " Mute" ), &v->m_mute ) );

        return schema_chan;
    }

    ControlContainerPtr addOutputChannel( ControlContainerPtr root, int chan, OutputProcessing *v )
    {
        string chan_name = formstring( chan + 1 );
        string description = formstring( "Output ", chan_name );

        ControlContainerPtr schema_chan = root->addItem( chan_name );

        schema_chan->addItem( "gain", makeGainControlDescriptor( formstring( description, " Gain" ), &v->m_gain ) );

        schema_chan->addItem( "mute", makeMuteControlDescriptor( formstring( description, " Mute" ), &v->m_mute ) );

        return schema_chan;
    }


    DescriptorPtr makeMuteControlDescriptor( string description, Mute *value )
    {
        return makeControlDescriptor(
            m_counts, AVDECC_AEM_CONTROL_TYPE_MUTE, description, AVDECC_CONTROL_VALUE_LINEAR_UINT8, ControlValue{"mute", value} );
    }

    DescriptorPtr makeGainControlDescriptor( string description, Gain *value )
    {
        return makeControlDescriptor(
            m_counts, AVDECC_AEM_CONTROL_TYPE_GAIN, description, AVDECC_CONTROL_VALUE_LINEAR_INT32, ControlValue{"gain", value} );
    }
};




bool testInteractiveTextConsole( Schema &schema, ControlIdentityComparatorPtr write_access )
{
    std::istringstream inputs;

    inputs.str(
                "\n"
                "/input/\\d+/gain\n"
                "+/input/1/mute\n"
                "+/input/2/mute\n"
                "+/input/3/mute\n"
                ":sleep\n"
                "?/input/1/mute\n"
                "?/input/1/gain\n"
                "/input/\\d+/mute\n"
                "+/input/\\d+/gain\n"
                "+\n"
                "/input/1/gain=-50.0\n"
                "/input/1/gain\n"
                "+/input/\\d+/mute\n"
                ":sleep\n"
                "+/input/2/mute\n"
                "/input/2/mute=1\n"
                "/input/2/gain=-10\n"
                "/input/1/gain=-40.0\n"
                ":sleep\n"
                "-/input/\\d+/gain\n"
                ":sleep\n"
                ":exit\n" );

    std::ostringstream result;

    interactiveTextConsole( result, inputs, schema, write_access, true );

    std::cout << result.str() << std::endl;
    return true;
}

int main( int argc, char **argv )
{
    bool r = true;
    {
        ProcessingSnapshot processing_settings;

        ControlContainerPtr top_controls = ControlContainer::create();

        DescriptorCounts counts;
        ExampleSchemaGenerator generator( top_controls, counts, &processing_settings );

        generator.generate();

        Schema schema( top_controls );
        std::shared_ptr<ControlIdentityComparatorNone> write_access = std::make_shared<ControlIdentityComparatorNone>();

        if ( argc > 1 )
        {
            interactiveTextConsole( std::cout, std::cin, schema, write_access );
        }
        else
        {
            testInteractiveTextConsole( schema, write_access );
        }
    }
    return r;
}
