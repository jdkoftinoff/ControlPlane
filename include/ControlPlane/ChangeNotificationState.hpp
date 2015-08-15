#pragma once

#include "World.hpp"
#include "ControlIdentityComparator.hpp"

namespace ControlPlane
{

using ChangeNotificationCallback
    = std::function<void(Milliseconds, ControlIdentityComparatorPtr const &, std::set<ControlIdentity> const &)>;

class ChangeNotificationState
{
  public:
    Milliseconds m_max_update_period_in_milliseconds;
    Milliseconds m_min_update_period_in_milliseconds;
    Milliseconds m_last_change_time_in_milliseconds;
    Milliseconds m_last_change_acknowledged_time_in_milliseconds;
    ChangeNotificationCallback m_callback;
    std::set<ControlIdentity> m_changed_items;

    friend std::ostream &operator<<( std::ostream &o, ChangeNotificationState const &v );
};
}
