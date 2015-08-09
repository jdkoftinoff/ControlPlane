#pragma once

#include "World.hpp"
#include "RangedValue.hpp"
#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"
#include "ControlDescriptor.hpp"
#include "MatrixDescriptor.hpp"
#include "ControlContainer.hpp"
#include "ChangeNotification.hpp"
#include "ChangeNotifierManager.hpp"

namespace ControlPlane
{

class Schema
{
    void collectDescriptors();

  public:
    Schema( ControlContainerPtr top_level ) : m_top_level( top_level ) { collectDescriptors(); }

    DescriptorPtr const getDescriptor( ControlIdentity const &identity ) const;

    DescriptorPtr getDescriptor( ControlIdentity const &identity );

    ControlIdentity getIdentityForAddress( SchemaAddress const &address ) const;

    const RangedValueBase *
        getRangedValueForDescriptor( ControlIdentity const &identity, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const;

    RangedValueBase *
        getRangedValueForDescriptor( ControlIdentity const &identity, int item_num = 0, int w_pos = 0, int h_pos = 0 );

    template <typename T>
    bool setValue( Milliseconds current_time_in_milliseconds,
                   T value,
                   ControlIdentity const &identity,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        bool changed = false;
        RangedValueBase *ranged_value = getRangedValueForDescriptor( identity, item_num, w_pos, h_pos );
        changed = ranged_value->setUnencodedValue( value );
        if ( changed )
        {
            m_change_manager.controlChanged( current_time_in_milliseconds, identity );
        }
        return changed;
    }

    bool setValue( Milliseconds current_time_in_milliseconds,
                   std::string value,
                   ControlIdentity const &identity,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        bool changed = false;
        RangedValueBase *ranged_value = getRangedValueForDescriptor( identity, item_num, w_pos, h_pos );
        changed = ranged_value->setUnencodedValueString( value );
        if ( changed )
        {
            m_change_manager.controlChanged( current_time_in_milliseconds, identity );
        }
        return changed;
    }

    template <typename T>
    bool setValue( Milliseconds current_time_in_milliseconds,
                   T value,
                   SchemaAddress const &address,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        return setValue( current_time_in_milliseconds, value, getIdentityForAddress( address ), item_num, w_pos, h_pos );
    }

    template <typename T>
    void getValue( T *value, ControlIdentity const &identity, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        RangedValueBase const *ranged_value = getRangedValueForDescriptor( identity, item_num, w_pos, h_pos );
        ranged_value->getUnencodedValue( value );
    }

    template <typename T>
    void getValue( T *value, SchemaAddress const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        getValue( value, getIdentityForAddress( address ), item_num, w_pos, h_pos );
    }

    ChangeNotifierManager &getChangeManager() { return m_change_manager; }
    ChangeNotifierManager const &getChangeManager() const { return m_change_manager; }

    std::recursive_mutex &getMutex() const { return m_access_mutex; }

    ControlContainerPtr getTop() { return m_top_level; }

    ControlContainerPtr const getTop() const { return m_top_level; }

  protected:
    ControlContainerPtr m_top_level;
    DescriptorAvdeccMap m_descriptor_avdecc_map;
    std::map<SchemaAddress, ControlIdentity> m_address_map;
    ChangeNotifierManager m_change_manager;
    mutable std::recursive_mutex m_access_mutex;
};
}
