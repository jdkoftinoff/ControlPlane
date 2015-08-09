#include "ControlPlane/World.hpp"
#include "ControlPlane/ChangeNotificationState.hpp"

namespace ControlPlane
{

std::ostream &operator<<( std::ostream &o, ChangeNotificationState const &v )
{
    o << "changed_items:" << std::endl;
    for ( auto const &i : v.m_changed_items )
    {
        o << " " << i << std::endl;
    }
    o << "last_change_acknowledged_time_in_milliseconds" << v.m_last_change_acknowledged_time_in_milliseconds.count()
      << std::endl;
    o << "last_change_time_in_milliseconds" << v.m_last_change_time_in_milliseconds.count() << std::endl;
    return o;
}
}
