#pragma once

#include "ControlPlane/World.hpp"
#include "ControlPlane/Schema.hpp"
#include "ControlPlane/ChangeNotification.hpp"

namespace ControlPlane
{

template <typename AddressT>
class SchemaAdaptor
{
  public:
    SchemaAdaptor( Schema &target ) : m_target( target ) { collectDescriptors(); }

    std::map<AddressT, ControlIdentity> const &getAddressMap() const { return m_address_map; }

    std::map<ControlIdentity, AddressT> const &getIdentityMap() const { return m_identity_map; }

    template <typename T>
    void getValue( T *value, AddressT const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        ControlIdentity identity;
        lookupIdentityForAddress( identity, address );
        m_target.getValue( value, identity, item_num, w_pos, h_pos );
    }

    template <typename T>
    bool setValue( Milliseconds current_time_in_milliseconds,
                   T value,
                   AddressT const &address,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        ControlIdentity identity;
        lookupIdentityForAddress( identity, address );
        return m_target.setValue( current_time_in_milliseconds, value, identity, item_num, w_pos, h_pos );
    }

    void lookupIdentityForAddress( ControlIdentity &identity, const AddressT &address ) const
    {
        std::lock_guard<std::recursive_mutex> lock( getMutex() );

        ControlIdentity r;
        auto i = m_address_map.find( address );
        if ( i != m_address_map.end() )
        {
            r = i->second;
        }
        else
        {
            throw std::runtime_error( Util::formstring( "No descriptor identity for text address:", address ) );
        }
        identity = r;
    }

    void lookupAddressForIdentity( AddressT &address, const ControlIdentity &identity ) const
    {
        std::lock_guard<std::recursive_mutex> lock( getMutex() );

        AddressT r;
        auto i = m_identity_map.find( identity );
        if ( i != m_identity_map.end() )
        {
            r = i->second;
        }
        else
        {
            throw std::runtime_error( Util::formstring( "No text address found for control identity:", identity ) );
        }
        address = r;
    }

    ChangeNotifierManager &getChangeManager() { return m_target.getChangeManager(); }

    ChangeNotifierManager const &getChangeManager() const { return m_target.getChangeManager(); }

    std::recursive_mutex &getMutex() const { return m_target.getMutex(); }

    ControlContainerPtr getTop() { return m_target.getTop(); }

    ControlContainerPtr const getTop() const { return m_target.getTop(); }

    Schema &getTarget() { return m_target; }

    uint64_t addChangeNotifier( ChangeNotifierPtr item ) { return getChangeManager().addChangeNotifier( item ); }

    void removeChangeNotifier( uint64_t identity ) { return getChangeManager().removeChangeNotifier( identity ); }

    void removeChangeNotifier( ChangeNotifierPtr item ) { return getChangeManager().removeChangeNotifier( item ); }

    class ChangeNotifierAdaptor : public ChangeNotifier
    {
      public:
        using Callback = std::function<void(std::set<AddressT> const &)>;

        ChangeNotifierAdaptor( SchemaAdaptor &schema_adaptor ) : m_schema_adaptor( schema_adaptor ) {}

        virtual ~ChangeNotifierAdaptor() {}

        void removeAddressSubscription( AddressT const &address )
        {
            ControlIdentity identity;
            m_schema_adaptor.lookupIdentityForAddress( identity, address );
            ChangeNotificationRange range( identity );

            removeSubscription( range );
        }

        void addAllSubscription( Milliseconds max_update_period_in_milliseconds,
                                 Milliseconds min_update_period_in_milliseconds,
                                 Callback callback )
        {
            ChangeNotificationRange range;
            range.setAll();

            addSubscription( range,
                             max_update_period_in_milliseconds,
                             min_update_period_in_milliseconds,
                             [=]( ChangeNotificationRange const &r, std::set<ControlIdentity> const &items )
                             {
                std::set<AddressT> address_items;
                for ( auto &i : items )
                {
                    AddressT a;
                    m_schema_adaptor.lookupAddressForIdentity( a, i );
                    address_items.insert( a );
                }
                callback( address_items );
            } );
        }

        void addAddressSubscription( AddressT const &address,
                                     Milliseconds max_update_period_in_milliseconds,
                                     Milliseconds min_update_period_in_milliseconds,
                                     Callback callback )
        {
            ControlIdentity identity;
            m_schema_adaptor.lookupIdentityForAddress( identity, address );
            ChangeNotificationRange range( identity );

            addSubscription( range,
                             max_update_period_in_milliseconds,
                             min_update_period_in_milliseconds,
                             [=]( ChangeNotificationRange const &r, std::set<ControlIdentity> const &items )
                             {
                std::set<AddressT> address_items;
                for ( auto &i : items )
                {
                    AddressT a;
                    m_schema_adaptor.lookupAddressForIdentity( a, i );
                    address_items.insert( a );
                }
                callback( address_items );
            } );
        }

      private:
        SchemaAdaptor &m_schema_adaptor;
    };

    using ChangeNotifierAdaptorPtr = shared_ptr<ChangeNotifierAdaptor>;

    ChangeNotifierAdaptorPtr createChangeNotifier()
    {
        ChangeNotifierAdaptorPtr r = make_shared<ChangeNotifierAdaptor>( *this );
        addChangeNotifier( r );
        return r;
    }

    class ChangeNotifierHolder
    {
        SchemaAdaptor<AddressT> &m_target;
        SchemaAdaptor<AddressT>::ChangeNotifierAdaptorPtr m_notifier;

      public:
        using ChangeNotifierAdaptorPtr = SchemaAdaptor<AddressT>::ChangeNotifierAdaptorPtr;
        ChangeNotifierHolder( SchemaAdaptor<AddressT> &target )
            : m_target( target ), m_notifier( target.createChangeNotifier() )
        {
        }

        ~ChangeNotifierHolder() { m_target.removeChangeNotifier( m_notifier ); }

        ChangeNotifierAdaptorPtr getNotifier() { return m_notifier; }
    };

  private:
    void collectDescriptors()
    {
        getTop()->enumerate( [=]( const SchemaAddress &schema_address, DescriptorPtr descriptor )
                             {
                                 ControlIdentity identity = descriptor->getControlIdentity();

                                 // Collect the top level mapping
                                 {
                                     AddressT address;
                                     getAddressForIdentity( address, identity, schema_address );
                                     m_address_map[address] = identity;
                                     m_identity_map[identity] = address;
                                 }

                                 // Collect the name value mapping
                                 for ( uint16_t name_num = 0; name_num < descriptor->getNumNames(); ++name_num )
                                 {
                                     AddressT address;
                                     ControlIdentity identity = descriptor->getControlIdentityForName( name_num );
                                     SchemaAddress name_schema_address = schema_address;
                                     name_schema_address.push_back( "name" );
                                     if ( descriptor->getNumNames() > 1 )
                                     {
                                         name_schema_address.push_back( Util::formstring( name_num + 1 ) );
                                     }
                                     getAddressForIdentity( address, identity, name_schema_address );
                                     m_address_map[address] = identity;
                                     m_identity_map[identity] = address;
                                 }

                                 // TODO: Handle matrix

                                 // Collect the value mapping if there is more than 1
                                 if ( descriptor->getNumValues() > 1 )
                                 {
                                     for ( uint16_t item_num = 0; item_num < descriptor->getNumValues(); ++item_num )
                                     {
                                         AddressT address;
                                         ControlIdentity identity = descriptor->getControlIdentityForItem( item_num );
                                         SchemaAddress item_schema_address = schema_address;
                                         item_schema_address.push_back( Util::formstring( item_num + 1 ) );
                                         getAddressForIdentity( address, identity, item_schema_address );
                                         m_address_map[address] = identity;
                                         m_identity_map[identity] = address;
                                     }
                                 }
                             } );
    }

    Schema &m_target;
    std::map<AddressT, ControlIdentity> m_address_map;
    std::map<ControlIdentity, AddressT> m_identity_map;
};
}
