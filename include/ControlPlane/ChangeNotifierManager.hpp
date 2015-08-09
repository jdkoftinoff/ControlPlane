#pragma once

#include "World.hpp"
#include "ChangeNotifier.hpp"

namespace ControlPlane
{

class ChangeNotifierManager
{
    friend class ChangeNotifierManagerHold;

  public:
    ChangeNotifierManager() : m_hold_count( 0 ) {}

    virtual ~ChangeNotifierManager() {}

    uint64_t addChangeNotifier( ChangeNotifierPtr item );

    void removeChangeNotifier( uint64_t identity );

    void removeChangeNotifier( ChangeNotifierPtr item ) { removeChangeNotifier( item->getIdentity() ); }

    void controlChanged( Milliseconds current_timestamp_in_milliseconds, ControlIdentity descriptor );

    void tick( Milliseconds current_timestamp_in_milliseconds );

    std::recursive_mutex &getMutex() const { return m_access_mutex; }

    std::map<uint64_t, ChangeNotifierPtr> const &getItems() const { return m_items; }

  private:
    void incHoldCount() { ++m_hold_count; }

    void decHoldCount() { --m_hold_count; }

    std::map<uint64_t, ChangeNotifierPtr> m_items;
    std::atomic<uint32_t> m_hold_count;
    mutable std::recursive_mutex m_access_mutex;
};

class ChangeNotifierManagerHold
{
    ChangeNotifierManager &m_mgr;

  public:
    ChangeNotifierManagerHold( ChangeNotifierManager &mgr ) : m_mgr( mgr ) { m_mgr.incHoldCount(); }

    ~ChangeNotifierManagerHold() { m_mgr.decHoldCount(); }
};
}
