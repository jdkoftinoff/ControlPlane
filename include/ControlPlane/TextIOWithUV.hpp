#pragma once

#include "World.hpp"
#include "Text.hpp"

#include "SchemaAdaptor.hpp"

#include <queue>

namespace ControlPlane
{
namespace Text
{
using namespace ControlPlane::Util;

using std::string;

class TextIOWithUV : public TextIO
{
    std::recursive_mutex m_mutex;
    std::queue<string> m_input;
    std::queue<string> m_output;
    size_t m_maxqueuesize;

  public:
    TextIOWithUV() : m_maxqueuesize( 3000 ) {}

    void sendLine( string const &line ) override
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        if ( m_output.size() < m_maxqueuesize )
        {
            m_output.push( line );
        }
    }

    bool receiveLine( string *result ) override
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        if ( m_input.size() != 0 )
        {
            result->clear();
            ( *result ) = m_input.front();
            m_input.pop();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool UVFromNetwork( string const &line )
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        if ( m_input.size() < m_maxqueuesize )
        {
            m_input.push( line );
            return true;
        }
        else
        {
            return false;
        }
    }

    bool UVToNework( string *result )
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        if ( m_output.size() != 0 )
        {
            result->clear();
            ( *result ) = m_output.front();
            m_output.pop();
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t UVToNetworkSize()
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        return m_output.size();
    }

    size_t UVFromNetworkSize()
    {
        std::lock_guard<std::recursive_mutex> hold( m_mutex );
        return m_input.size();
    }
};

} // Text
} // ControlPlane
