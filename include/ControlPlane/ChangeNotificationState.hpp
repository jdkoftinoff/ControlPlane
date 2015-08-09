#pragma once

#include "World.hpp"
#include "ChangeNotificationRange.hpp"

namespace ControlPlane
{

using ChangeNotificationCallback = std::function<void(ChangeNotificationRange const &, std::set<ControlIdentity> const &)>;

class ChangeNotificationState
{
  public:
    Milliseconds m_last_change_time_in_milliseconds;
    Milliseconds m_last_change_acknowledged_time_in_milliseconds;
    ChangeNotificationCallback m_callback;
    std::set<ControlIdentity> m_changed_items;

    friend std::ostream &operator<<( std::ostream &o, ChangeNotificationState const &v );
};
}
