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

struct DeviceInfo
{
    Descriptor::EntityInfo m_entity;
};

class ExampleSchemaGenerator : public SchemaGenerator
{
    Descriptor::DescriptorCounts m_counts;
    ProcessingSnapshot *m_processing_settings;
    DeviceInfo *m_device_info;
    ControlIdentityComparatorSetPtr m_write_access;

  public:
    ExampleSchemaGenerator( ControlContainerPtr root,
                            ProcessingSnapshot *processing_settings,
                            ControlIdentityComparatorSetPtr write_access )
        : SchemaGenerator( root, m_counts ), m_processing_settings( processing_settings ), m_write_access( write_access )
    {
    }

    ControlContainerPtr generate() override
    {
        Descriptor::EntityPtr entity = makeEntity( "Example Entity", &m_device_info->m_entity );
        Descriptor::ConfigurationPtr configuration = Descriptor::makeConfiguration( "Default" );

        ControlContainerPtr schema_input = m_root->addItem( "input" );

        for ( size_t chan = 0; chan < m_processing_settings->m_input.size(); ++chan )
        {
            addInputChannel( schema_input, configuration, chan, &m_processing_settings->m_input[chan] );
        }

        ControlContainerPtr schema_output = m_root->addItem( "output" );

        for ( size_t chan = 0; chan < m_processing_settings->m_output.size(); ++chan )
        {
            addOutputChannel( schema_output, configuration, chan, &m_processing_settings->m_output[chan] );
        }

        entity->addChildDescriptor( configuration );
        entity->collectOwnedDescriptors( m_counts, m_root );

        return m_root;
    }

    ControlContainerPtr
        addInputChannel( ControlContainerPtr root, Descriptor::DescriptorPtr owner, int chan, InputProcessing *v )
    {
        string chan_name = formstring( chan + 1 );
        string description = formstring( "Input ", chan_name );

        ControlContainerPtr schema_chan = root->addItem( chan_name );

        schema_chan->addItem( "gain", makeGainControlDescriptor( owner, formstring( description, " Gain" ), &v->m_gain ) );

        schema_chan->addItem( "mute", makeMuteControlDescriptor( owner, formstring( description, " Mute" ), &v->m_mute ) );

        return schema_chan;
    }

    ControlContainerPtr
        addOutputChannel( ControlContainerPtr root, Descriptor::DescriptorPtr owner, int chan, OutputProcessing *v )
    {
        string chan_name = formstring( chan + 1 );
        string description = formstring( "Output ", chan_name );

        ControlContainerPtr schema_chan = root->addItem( chan_name );

        schema_chan->addItem( "gain", makeGainControlDescriptor( owner, formstring( description, " Gain" ), &v->m_gain ) );

        schema_chan->addItem( "mute", makeMuteControlDescriptor( owner, formstring( description, " Mute" ), &v->m_mute ) );

        return schema_chan;
    }

    DescriptorPtr makeMuteControlDescriptor( Descriptor::DescriptorPtr &owner, string description, Mute *value )
    {
        Descriptor::ControlPtr control = Descriptor::makeControl(
            AVDECC_AEM_CONTROL_TYPE_MUTE, description, AVDECC_CONTROL_VALUE_LINEAR_UINT8, ControlValue{"mute", value} );
        owner->addChildDescriptor( control );
        return control;
    }

    DescriptorPtr makeGainControlDescriptor( Descriptor::DescriptorPtr &owner, string description, Gain *value )
    {
        Descriptor::ControlPtr control = Descriptor::makeControl(
            AVDECC_AEM_CONTROL_TYPE_GAIN, description, AVDECC_CONTROL_VALUE_LINEAR_INT32, ControlValue{"gain", value} );
        owner->addChildDescriptor( control );
        return control;
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

        ControlIdentityComparatorSetPtr write_access = std::make_shared<ControlIdentityComparatorSet>();
        ExampleSchemaGenerator generator( top_controls, &processing_settings, write_access );

        generator.generate();

        Schema schema( top_controls );
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
