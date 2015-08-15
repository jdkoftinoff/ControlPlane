#pragma once

#include "World.hpp"
#include "Text.hpp"

#include "SchemaAdaptor.hpp"

namespace ControlPlane
{
namespace Text
{
using namespace ControlPlane::Util;

using std::string;

class TextIOWithStream : public TextIO
{
    std::recursive_mutex m_mutex;
    std::istream &m_input;
    std::ostream &m_output;

  public:
    TextIOWithStream( std::istream &input, std::ostream &output ) : m_input( input ), m_output( output ) {}

    void sendLine( string const &line ) override
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        m_output << line << std::endl;
    }

    bool receiveLine( string *result ) override
    {
        bool r = false;
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        result->clear();
        std::getline( m_input, *result );
        if ( result->length() > 0 )
        {
            r = true;
        }
        return r;
    }
};

inline void interactiveTextConsole( std::ostream &output,
                                    std::istream &input,
                                    Schema &schema,
                                    ControlIdentityComparatorPtr write_access = 0,
                                    bool echo = false )
{
    bool done = false;

    try
    {
        SchemaTextAdaptor text_schema( schema );
        TextIOWithStream text_io( input, output );
        TextProtocolSession session( text_io, text_schema, write_access, echo );

        Milliseconds current_time_in_milliseconds = getCurrentTimeInMilliseconds();

        do
        {
            string line;
            text_io.receiveLine( &line );

            text_schema.getChangeManager().tick( current_time_in_milliseconds );

            current_time_in_milliseconds = getCurrentTimeInMilliseconds();

            done = session.handleLine( current_time_in_milliseconds, line );

        } while ( !done );
    }
    catch ( SchemaError const &e )
    {
        output << "Exception Caught SchemaError: " << e.what() << std::endl;
    }
    catch ( std::runtime_error const &e )
    {
        output << "Exception Caught runtime_error: " << e.what() << std::endl;
    }
    catch ( std::logic_error const &e )
    {
        output << "Exception Caught logic_error: " << e.what() << std::endl;
    }
    catch ( std::exception const &e )
    {
        output << "Exception Caught: " << e.what() << std::endl;
    }
}
}
}
