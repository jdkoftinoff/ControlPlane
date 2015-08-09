#include "ControlPlane/World.hpp"
#include "ControlPlane/ChangeNotifier.hpp"

namespace ControlPlane
{
std::atomic<uint64_t> ChangeNotifier::global_identity_count;

ChangeNotifier::ChangeNotifier( Milliseconds min_scan_period_in_milliseconds )
    : m_identity( global_identity_count++ )
    , m_min_scan_period_in_milliseconds( min_scan_period_in_milliseconds )
    , m_last_scan_time_in_milliseconds( 0 )
{
}

void ChangeNotifier::removeSubscription( ChangeNotificationRange sub )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    auto i = m_subscriptions.find( sub );
    if ( i != m_subscriptions.end() )
    {
        m_subscriptions.erase( i );
    }
}

void ChangeNotifier::addSubscription( ChangeNotificationRange sub,
                                      Milliseconds max_update_period_in_milliseconds,
                                      Milliseconds min_update_period_in_milliseconds,
                                      ChangeNotificationCallback callback )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );

    m_subscriptions[sub]
        = ChangeNotificationState{max_update_period_in_milliseconds, min_update_period_in_milliseconds, callback};
}

void ChangeNotifier::controlChanged( Milliseconds current_timestamp_in_milliseconds, ControlIdentity descriptor_identity )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    for ( auto &sub : m_subscriptions )
    {
        if ( sub.first.containsDescriptor( descriptor_identity ) )
        {
            sub.second.m_last_change_time_in_milliseconds = current_timestamp_in_milliseconds;
            sub.second.m_changed_items.insert( descriptor_identity );
        }
    }
}

void ChangeNotifier::tick( Milliseconds current_timestamp_in_milliseconds )
{
    std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
    // Do work only after m_min_scan_period_in_milliseconds since last time
    if ( current_timestamp_in_milliseconds > m_last_scan_time_in_milliseconds + m_min_scan_period_in_milliseconds )
    {
        // remember the current time as when we last did any work
        m_last_scan_time_in_milliseconds = current_timestamp_in_milliseconds;
        for ( auto &sub : m_subscriptions )
        {
            bool notify = false;
            ChangeNotificationRange const &range = sub.first;
            ChangeNotificationState &state = sub.second;

            // notify the callback if no notification has happened since
            // range.m_last_change_acknowledged_time_in_milliseconds
            if ( ( current_timestamp_in_milliseconds - state.m_last_change_acknowledged_time_in_milliseconds )
                 >= range.m_min_update_period_in_milliseconds )
            {
                notify = true;
            }
            else
            {
                // Did something change since the last time we called the
                // callback?
                if ( state.m_last_change_time_in_milliseconds > state.m_last_change_acknowledged_time_in_milliseconds )
                {
                    // Yes, was it changed at least
                    // state.m_max_update_period_in_milliseconds ago?
                    if ( ( current_timestamp_in_milliseconds - range.m_max_update_period_in_milliseconds )
                         >= state.m_last_change_acknowledged_time_in_milliseconds )
                    {
                        // yes, do the notify
                        notify = true;
                    }
                }
            }

            // if there is a reason to notify the callback, do so now, and
            // remember when we did it
            if ( notify )
            {
                state.m_callback( range, state.m_changed_items );
                state.m_last_change_acknowledged_time_in_milliseconds = current_timestamp_in_milliseconds;
                state.m_changed_items.clear();
            }
        }
    }
}

std::ostream &operator<<( std::ostream &o, const ChangeNotifier &v )
{
    o << v.m_last_scan_time_in_milliseconds.count() << " " << v.m_min_scan_period_in_milliseconds.count() << std::endl;
    for ( auto const &i : v.m_subscriptions )
    {
        o << i.first << std::endl;
        o << i.second << std::endl;
    }
    return o;
}
}
