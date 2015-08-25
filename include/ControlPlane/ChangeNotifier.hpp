#pragma once

#include "World.hpp"
#include "ControlIdentityComparator.hpp"
#include "ChangeNotificationState.hpp"

namespace ControlPlane
{

class ChangeNotifier
{
  public:
    ChangeNotifier( Milliseconds min_scan_period_in_milliseconds = Milliseconds( 30 ) );

    virtual ~ChangeNotifier() {}

    uint64_t getIdentity() const { return m_identity; }

    void removeSubscription( ControlIdentityComparatorPtr sub );

    void addSubscription( ControlIdentityComparatorPtr sub,
                          Milliseconds max_update_period_in_milliseconds,
                          Milliseconds min_update_period_in_milliseconds,
                          Milliseconds current_time_in_milliseconds,
                          ChangeNotificationCallback callback );

    void controlChanged( Milliseconds current_timestamp_in_milliseconds, ControlIdentity descriptor );

    void tick( Milliseconds current_timestamp_in_milliseconds );

  protected:
    mutable std::recursive_mutex m_access_mutex;

  private:
    static std::atomic<uint64_t> global_identity_count;

    uint64_t m_identity;

    Milliseconds m_min_scan_period_in_milliseconds;
    Milliseconds m_last_scan_time_in_milliseconds;

    std::map<ControlIdentityComparatorPtr, ChangeNotificationState, ControlIdentityComparator_compare> m_subscriptions;

    friend std::ostream &operator<<( std::ostream &o, const ChangeNotifier &v );
};

using ChangeNotifierPtr = shared_ptr<ChangeNotifier>;
}
