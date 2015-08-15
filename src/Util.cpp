#include "ControlPlane/World.hpp"
#include "ControlPlane/Util.hpp"

namespace ControlPlane
{
namespace Util
{

string escapeString( const string &raw )
{
    string printable;
    size_t length_to_use = raw.length();
    for ( size_t i = 0; i < length_to_use; ++i )
    {
        char c = raw[i];
        if ( c == 0 )
        {
            break;
        }
        if ( c == '\\' )
        {
            printable.push_back( '\\' );
        }
        else if ( c == '"' )
        {
            printable.append( "\\\"" );
        }
        else if ( c == '\r' )
        {
            printable.append( "\\r" );
        }
        else if ( c == '\n' )
        {
            printable.append( "\\n" );
        }
        else if ( c == '\t' )
        {
            printable.append( "\\t" );
        }
        else if ( c < ' ' && c >= 0 )
        {
            printable.append( "\\x" );
            char hex[] = "0123456789abcdef";
            printable.append( 1, hex[( c >> 4 ) & 0xf] );
            printable.append( 1, hex[( c & 0xf )] );
        }
        else
        {
            printable.push_back( c );
        }
    }
    return printable;
}

string unEscapeString( string const &v )
{
    std::istringstream i( v );
    string r;
    bool single_quote;

    i >> std::noskipws;

    char c;

    enum
    {
        STATE_BEGIN,
        STATE_PLAIN,
        STATE_END,
        STATE_ESCAPE,
        STATE_FIRST_X,
        STATE_SECOND_X
    } state = STATE_BEGIN;

    char tmp_x = 0;

    while ( state != STATE_END && ( i >> c ) )
    {
        switch ( state )
        {
        case STATE_BEGIN:
        {
            // search for beginning double quote or single quote. skip whitespace. error if not whitespace.
            switch ( c )
            {
            case '\'':
                state = STATE_PLAIN;
                single_quote = true;
                break;
            case '"':
                state = STATE_PLAIN;
                single_quote = false;
                break;
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                break;
            default:
                throw std::runtime_error( "unEncodeString parse error" );
                return r;
            }
            break;
        }
        case STATE_PLAIN:
        {
            // handle plain text.
            // backslash goes into escape state. double quote ends
            // string.
            switch ( c )
            {
            case '\\':
                state = STATE_ESCAPE;
                break;
            case '\'':
                if ( single_quote == true )
                {
                    state = STATE_END;
                }
                break;
            case '"':
                if ( single_quote == false )
                {
                    state = STATE_END;
                }
                break;
            default:
                r.push_back( c );
                break;
            }
            break;
        }
        case STATE_ESCAPE:
        {
            // handle escape mode.
            // backslash backslash is code for single backslash.
            // backslash n is code for line feed
            // backslash r is code for carriage return
            // backslash t is code for tab
            // backslash " is code for "
            // backslash x is code for two digit hex code, going into
            // first_x
            // state
            // any other code is error
            switch ( c )
            {
            case '\\':
                r.push_back( '\\' );
                state = STATE_PLAIN;
                break;
            case '"':
                r.push_back( '"' );
                state = STATE_PLAIN;
                break;
            case 'n':
                r.push_back( '\n' );
                state = STATE_PLAIN;
                break;
            case 'r':
                r.push_back( '\r' );
                state = STATE_PLAIN;
                break;
            case 't':
                r.push_back( '\t' );
                state = STATE_PLAIN;
                break;
            case 'x':
                state = STATE_FIRST_X;
                break;
            default:
                i.clear( std::ios_base::failbit );
                throw std::runtime_error( "unEncodeString parse error" );
                return r;
            }
            break;
        }
        case STATE_FIRST_X:
        {
            if ( c >= '0' && c <= '9' )
            {
                tmp_x = ( c - '0' ) << 4;
                state = STATE_SECOND_X;
            }
            else if ( c >= 'a' && c <= 'f' )
            {
                tmp_x = ( c - 'a' + 10 ) << 4;
                state = STATE_SECOND_X;
            }
            else if ( c >= 'A' && c <= 'F' )
            {
                tmp_x = ( c - 'A' + 10 ) << 4;
                state = STATE_SECOND_X;
            }
            else
            {
                i.clear( std::ios_base::failbit );
                throw std::runtime_error( "unEncodeString parse error" );
                return r;
            }
            break;
        }
        case STATE_SECOND_X:
        {
            if ( c >= '0' && c <= '9' )
            {
                tmp_x |= c - '0';
                r.push_back( tmp_x );
                state = STATE_PLAIN;
            }
            else if ( c >= 'a' && c <= 'f' )
            {
                tmp_x |= c - 'a' + 10;
                r.push_back( tmp_x );
                state = STATE_PLAIN;
            }
            else if ( c >= 'A' && c <= 'F' )
            {
                tmp_x |= c - 'A' + 10;
                r.push_back( tmp_x );
                state = STATE_PLAIN;
            }
            else
            {
                i.clear( std::ios_base::failbit );
                throw std::runtime_error( "unEncodeString parse error" );
                return r;
            }
            break;
        }
        case STATE_END:
        {
            break;
        }
        }
    }
    return r;
}
}
}
