#include "ControlPlane/World.hpp"
#include "ControlPlane/ChangeNotifierManager.hpp"

namespace ControlPlane
{

uint64_t ChangeNotifierManager::addChangeNotifier( ChangeNotifierPtr item )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    uint64_t identity = item->getIdentity();

    m_items[identity] = item;
    return identity;
}

void ChangeNotifierManager::removeChangeNotifier( uint64_t identity )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    auto i = m_items.find( identity );
    if ( i != m_items.end() )
    {
        m_items.erase( i );
    }
}

void ChangeNotifierManager::controlChanged( Milliseconds current_timestamp_in_milliseconds, ControlIdentity descriptor )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    for ( auto &i : m_items )
    {
        i.second->controlChanged( current_timestamp_in_milliseconds, descriptor );
    }
}

void ChangeNotifierManager::tick( Milliseconds current_timestamp_in_milliseconds )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    if ( m_hold_count == 0 )
    {
        for ( auto &i : m_items )
        {
            i.second->tick( current_timestamp_in_milliseconds );
        }
    }
}
}
