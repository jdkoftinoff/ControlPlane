#include "ControlPlane/World.hpp"
#include "ControlPlane/Text.hpp"

namespace ControlPlane
{
namespace Text
{

using namespace ControlPlane::Util;

void getAddressForIdentity( TextAddress &address, const ControlIdentity &identity, const SchemaAddress &schema_address )
{
    string r = "/";
    size_t count = schema_address.size();

    for ( size_t i = 0; i < count; ++i )
    {
        r += schema_address[i];
        if ( i != count - 1 )
        {
            r += "/";
        }
    }
    address = TextAddress{r};
}

bool TextProtocolSession::handleLine( Milliseconds current_time_in_milliseconds, const string &line )
{
    bool done = false;

    try
    {
        if ( m_echo )
        {
            m_io.sendLine( formstring( ">", line ) );
        }
        if ( line == ":quit" || line == ":exit" )
        {
            done = true;
        }
        else if ( line.length() > 0 )
        {
            if ( line[0] == '#' )
            {
                // do nothing for comment
            }
            else if ( ( isalnum( line[0] ) || line[0] == '/' ) && line.find_first_of( '=' ) == string::npos )
            {
                handleGet( current_time_in_milliseconds, line );
            }
            else if ( line[0] == '?' )
            {
                handleDescribe( current_time_in_milliseconds, line.substr( 1 ) );
            }
            else if ( line[0] == '+' )
            {
                handleSubscribe( current_time_in_milliseconds, line.substr( 1 ) );
            }
            else if ( line[0] == '-' )
            {
                handleUnsubscribe( current_time_in_milliseconds, line.substr( 1 ) );
            }
            else if ( line[0] == ':' )
            {
                handleOtherCommands( current_time_in_milliseconds, line.substr( 1 ) );
            }
            else if ( line.find_first_of( '=' ) != string::npos )
            {
                handleSet( current_time_in_milliseconds, line );
            }
        }
    }
    catch ( std::runtime_error const &e )
    {
        m_io.sendLine( formstring( "# Error: ", line, " : ", e.what() ) );
    }

    return done;
}

void TextProtocolSession::handleGet( Milliseconds current_time_in_milliseconds, const string &line )
{
    std::vector<string> params;
    split( params, line, " ", splitmode::no_empties );

    TextAddress address;

    if ( params.size() > 0 )
    {
        address.m_value = normalizeAddress( params[0] );
    }

    if ( params.size() > 0 )
    {
        if ( doesContainRegex( address.m_value ) )
        {
            TextAddressSet items = getAddressesForRegex( address.m_value );
            for ( auto const &i : items )
            {
                handleIndividualGet( current_time_in_milliseconds, i );
            }
        }
        else
        {
            handleIndividualGet( current_time_in_milliseconds, address );
        }
    }
}

void TextProtocolSession::handleSet( Milliseconds current_time_in_milliseconds, const string &line )
{
    std::vector<string> params;
    split( params, line, "=", splitmode::no_empties );

    TextAddress address;

    if ( params.size() > 0 )
    {
        address.m_value = normalizeAddress( params[0] );
    }

    if ( params.size() > 1 )
    {
        if ( doesContainRegex( address.m_value ) )
        {
            TextAddressSet items = getAddressesForRegex( address.m_value );
            ControlPlane::ChangeNotifierManagerHold hold_notifications_for_other_subscribersp( m_notifier.getManager() );

            for ( auto const &i : items )
            {
                handleIndividualSet( current_time_in_milliseconds, i, params[1] );
            }
        }
        else
        {
            handleIndividualSet( current_time_in_milliseconds, address, params[1] );
        }
    }
}

void TextProtocolSession::handleDescribe( Milliseconds current_time_in_milliseconds, const string &line )
{
    std::vector<string> params;
    split( params, line, " ", splitmode::no_empties );

    TextAddress address;

    if ( params.size() > 0 )
    {
        address.m_value = normalizeAddress( params[0] );
    }

    if ( params.size() > 0 )
    {
        if ( doesContainRegex( address.m_value ) )
        {
            TextAddressSet items = getAddressesForRegex( address.m_value );
            for ( auto const &i : items )
            {
                handleIndividualDescribe( current_time_in_milliseconds, i );
            }
        }
        else
        {
            handleIndividualDescribe( current_time_in_milliseconds, address );
        }
    }
}

void TextProtocolSession::handleSubscribe( Milliseconds current_time_in_milliseconds, const string &line )
{
    std::vector<string> params;
    split( params, line, " ", splitmode::no_empties );

    TextAddress subscription_address;
    Milliseconds min_update_period{30};
    Milliseconds max_update_period{10000};

    if ( params.size() > 0 )
    {
        subscription_address.m_value = normalizeAddress( params[0] );
    }
    if ( params.size() > 1 )
    {
        int64_t v;
        lexical_cast( v, params[1] );
        min_update_period = Milliseconds( v );
    }
    if ( params.size() > 2 )
    {
        int64_t v;
        lexical_cast( v, params[1] );
        max_update_period = Milliseconds( v );
    }

    if ( params.size() > 0 )
    {
        if ( doesContainRegex( subscription_address.m_value ) )
        {
            TextAddressSet items = getAddressesForRegex( subscription_address.m_value );
            for ( auto const &i : items )
            {
                handleIndividualSubscribe( current_time_in_milliseconds, i, min_update_period, max_update_period );
            }
        }
        else
        {
            handleIndividualSubscribe(
                current_time_in_milliseconds, subscription_address, min_update_period, max_update_period );
        }
    }
}

void TextProtocolSession::handleUnsubscribe( Milliseconds current_time_in_milliseconds, const string &line )
{
    string normalized = normalizeAddress( line );
    if ( doesContainRegex( normalized ) )
    {
        TextAddressSet items = getAddressesForRegex( normalized );
        for ( auto const &i : items )
        {
            handleIndividualUnsubscribe( current_time_in_milliseconds, i );
        }
    }
    else
    {
        handleIndividualUnsubscribe( current_time_in_milliseconds, TextAddress( normalized ) );
    }
}

void TextProtocolSession::handleOtherCommands( Milliseconds current_time_in_milliseconds, const string &line )
{
    std::vector<string> params;
    split( params, line, " ", splitmode::no_empties );
    if ( params.size() > 0 )
    {
        if ( params[0] == "sleep" )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        }
        else
        {
            throw( std::runtime_error( "Bad command: " + params[0] ) );
        }
    }
}

void TextProtocolSession::handleIndividualGet( Milliseconds current_time_in_milliseconds, const TextAddress &address )
{
    string r;
    m_schema.getValue( &r, address );
    if ( m_schema.isStorageTypeString( address ) )
    {
        m_io.sendLine( formstring( address, "='", Util::escapeString( r ), "'" ) );
    }
    else
    {
        m_io.sendLine( formstring( address, "=", r ) );
    }
}

void TextProtocolSession::handleIndividualSet( Milliseconds current_time_in_milliseconds,
                                               const TextAddress &address,
                                               const string &v )
{
    if ( m_schema.isStorageTypeString( address ) )
    {
        string unescaped = unEscapeString( v );
        m_schema.setValue( m_write_access, current_time_in_milliseconds, unescaped, address );
    }
    else
    {
        m_schema.setValue( m_write_access, current_time_in_milliseconds, v, address );
    }

    handleIndividualGet( current_time_in_milliseconds, address );
}

void TextProtocolSession::handleIndividualDescribe( Milliseconds current_time_in_milliseconds, const TextAddress &address )
{
    std::lock_guard<std::recursive_mutex> lock( m_schema.getTarget().getMutex() );

    ControlIdentity identity;

    m_schema.lookupIdentityForAddress( identity, address );

    std::stringstream response;
    response << formstring( "?{'", address, "'" );

    if ( identity.m_section == ControlIdentity::SectionDescriptorLevel )
    {
        DescriptorPtr d = m_schema.getTarget().getDescriptor( identity );

        response << ": [";

        for ( size_t item = 0; item < d->getNumValues(); ++item )
        {
            ControlValue const &v = d->getValue( item );

            response << describeRangedValue( v.m_name, *v.m_ranged_value );

            if ( item < d->getNumValues() - 1 )
            {
                response << ", ";
            }
        }
        response << "]";
    }
    else if ( identity.m_section == ControlIdentity::SectionName )
    {
        const RangedValueBase *v = m_schema.getTarget().getRangedValueForControlIdentity( identity );
        response << ":" << describeRangedValue( formstring( "name_", identity.m_item + 1 ), *v );
    }
    else
    {
        const RangedValueBase *v = m_schema.getTarget().getRangedValueForControlIdentity( identity );
        response << ":" << describeRangedValue( formstring( "item_", identity.m_h_pos + 1, "_", identity.m_w_pos + 1 ), *v );
    }
    response << "}";
    m_io.sendLine( response.str() );
}

void TextProtocolSession::handleIndividualSubscribe( Milliseconds current_time_in_milliseconds,
                                                     const TextAddress &address,
                                                     Milliseconds min_time,
                                                     Milliseconds max_time )
{
    m_notifier.getNotifier()->addAddressSubscription( address,
                                                      min_time,
                                                      max_time,
                                                      current_time_in_milliseconds,
                                                      [&]( Milliseconds changed_time, TextAddressSet const &items )
                                                      {
        for ( auto &i : items )
        {
            handleIndividualGet( current_time_in_milliseconds, i );
        }
    } );
}

void TextProtocolSession::handleIndividualUnsubscribe( Milliseconds current_time_in_milliseconds, const TextAddress &address )
{
    m_notifier.getNotifier()->removeAddressSubscription( address );
    m_io.sendLine( formstring( "-", address ) );
}

string TextProtocolSession::describeRangedValue( const std::string &name, const RangedValueBase &v )
{
    string response;
    if ( v.getStorageType() == EncodingType::ENCODING_STRING64 || v.getStorageType() == EncodingType::ENCODING_STRING406 )
    {
        response = formstring( "[ { 'value' : '", v.getUnencodedValueString( false ), "'", "} ]" );
    }
    else
    {
        response = formstring(
            "{"
            " 'name' : '",
            name,
            "', 'value' : '",
            v.getUnencodedValueString( false ),
            "', 'minimum' : '",
            v.getUnencodedMinimumString( false ),
            "', 'maximum' : '",
            v.getUnencodedMaximumString( false ),
            "', 'default' : '",
            v.getUnencodedDefaultString( false ),
            "', 'step' : '",
            v.getUnencodedStepString( false ),
            "', 'units' : '",
            v.getUnitsSuffix(),
            "'",
            "}" );
    }
    return response;
}

TextProtocolSession::TextAddressSet TextProtocolSession::getAddressesForRegex( const string &regex_string )
{
    TextAddressSet r;

    std::regex reg( formstring( "^", regex_string, "$" ), std::regex_constants::ECMAScript );

    for ( auto const &i : m_schema.getAddressMap() )
    {
        string const &v = i.first.m_value;

        if ( std::regex_search( v, reg ) )
        {
            r.insert( i.first );
        }
    }

    return r;
}

bool TextProtocolSession::doesContainRegex( const string &s ) { return s.find_first_of( "\\:[]{}|+().^$*?" ) != string::npos; }

string TextProtocolSession::normalizeAddress( const string &s )
{
    string r = s;
#if 0
    r.erase( std::remove_if( r.begin(),
                             r.end(),
                             []( char c )
                             {
                 return ( c == '\r' || c == '\t' || c == ' ' || c == '\n' );
             } ),
             s.end() );
#endif
    return r;
}
}
}
