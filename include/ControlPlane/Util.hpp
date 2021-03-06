#pragma once

#include "World.hpp"

namespace ControlPlane
{
namespace Util
{

string escapeString( string const &raw );
string unEscapeString( string const &v );

template <typename OutT, typename InT>
void lexical_cast( OutT &r, InT const &v )
{
    std::stringstream buf;
    buf << v;
    buf >> r;
}

template <typename FirstArg>
void emit_formstring( std::stringstream &result, FirstArg &&first )
{
    result << first;
}

template <typename FirstArg, typename... Args>
void emit_formstring( std::stringstream &result, FirstArg &&first, Args &&... args )
{
    result << first;
    emit_formstring( result, args... );
}

template <typename... Args>
std::string formstring( Args &&... args )
{
    std::stringstream ostr;
    emit_formstring( ostr, args... );
    return ostr.str();
}

struct splitmode
{
    enum empties_t
    {
        empties_ok,
        no_empties
    };
};

template <typename Container>
Container &split( Container &result,
                  const typename Container::value_type &s,
                  const typename Container::value_type &delimiters,
                  splitmode::empties_t empties = splitmode::empties_ok )
{
    result.clear();
    size_t current;
    size_t next = -1;
    do
    {
        if ( empties == splitmode::no_empties )
        {
            next = s.find_first_not_of( delimiters, next + 1 );
            if ( next == Container::value_type::npos )
                break;
            next -= 1;
        }
        current = next + 1;
        next = s.find_first_of( delimiters, current );
        result.push_back( s.substr( current, next - current ) );
    } while ( next != Container::value_type::npos );
    return result;
}

inline Milliseconds getCurrentTimeInMilliseconds()
{
    auto duration = std::chrono::steady_clock::now().time_since_epoch();
    Milliseconds current_time_in_milliseconds = std::chrono::duration_cast<Milliseconds>( duration );
    return current_time_in_milliseconds;
}

inline void copySockaddr( sockaddr_storage *result, sockaddr const *input )
{
    memset( result, 0, sizeof( sockaddr_storage ) );
    if ( input->sa_family == AF_INET )
    {
        memcpy( result, input, sizeof( sockaddr_in ) );
    }
    else if ( input->sa_family == AF_INET6 )
    {
        memcpy( result, input, sizeof( sockaddr_in6 ) );
    }
}

struct Less_sockaddr_storage
{
    bool operator()( const sockaddr_storage &lhs, const sockaddr_storage &rhs ) const
    {
        return memcmp( &lhs, &rhs, sizeof( lhs ) ) < 0;
    }
};

struct Less_sockaddr_in
{
    bool operator()( const sockaddr_in &lhs, const sockaddr_in &rhs ) const { return memcmp( &lhs, &rhs, sizeof( lhs ) ) < 0; }
};

struct Less_sockaddr_in6
{
    bool operator()( const sockaddr_in6 &lhs, const sockaddr_in6 &rhs ) const
    {
        return memcmp( &lhs, &rhs, sizeof( lhs ) ) < 0;
    }
};
}
}
