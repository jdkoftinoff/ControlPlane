#pragma once

#include "ControlPlane/World.hpp"
#include "ControlPlane/Schema.hpp"
#include "ControlPlane/ChangeNotification.hpp"

namespace ControlPlane
{

class SchemaAdaptorBase
{
  public:
    SchemaAdaptorBase( Schema &target ) : m_target( target ) {}
    virtual ~SchemaAdaptorBase() {}

    ChangeNotifierManager &getChangeManager() { return m_target.getChangeManager(); }

    ChangeNotifierManager const &getChangeManager() const { return m_target.getChangeManager(); }

    std::recursive_mutex &getMutex() const { return m_target.getMutex(); }

    ControlContainerPtr getTop() { return m_target.getTop(); }

    ControlContainerPtr const getTop() const { return m_target.getTop(); }

    Schema &getTarget() { return m_target; }

    uint64_t addChangeNotifier( ChangeNotifierPtr item ) { return getChangeManager().addChangeNotifier( item ); }

    void removeChangeNotifier( uint64_t identity ) { return getChangeManager().removeChangeNotifier( identity ); }

    void removeChangeNotifier( ChangeNotifierPtr item ) { return getChangeManager().removeChangeNotifier( item ); }

    virtual void collectDescriptors();

  protected:
    virtual void collectDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor );

    virtual void collectEntityDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor );

    virtual void collectMatrixDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor );

    virtual void collectControlDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor );

    virtual void collectOtherDescriptor( const SchemaAddress &schema_address, DescriptorPtr descriptor );

    virtual void collectItem( ControlIdentity const &identity, SchemaAddress const &schema_address ) = 0;

    Schema &m_target;
};

template <typename AddressT>
class SchemaAdaptor : public SchemaAdaptorBase
{
  public:
    class SchemaAdaptorErrorReadOnly : public SchemaError
    {
      public:
        ControlIdentity m_identity;
        AddressT m_address;

        SchemaAdaptorErrorReadOnly( ControlIdentity const &identity, AddressT const &address )
            : SchemaError( Util::formstring( "SchemaAdaptorErrorReadOnly:", address ) )
            , m_identity( identity )
            , m_address( address )
        {
        }
    };

    class SchemaAdaptorErrorAddressNotFound : public SchemaError
    {
      public:
        AddressT m_address;

        SchemaAdaptorErrorAddressNotFound( AddressT const &address )
            : SchemaError( Util::formstring( "SchemaAdaptorErrorAddressNotFound:", address ) ), m_address( address )
        {
        }
    };

    SchemaAdaptor( Schema &target ) : SchemaAdaptorBase( target ) { collectDescriptors(); }

    std::map<AddressT, ControlIdentity> const &getAddressMap() const { return m_address_map; }

    std::map<ControlIdentity, AddressT> const &getIdentityMap() const { return m_identity_map; }

    EncodingType getEncodingTypeForAddress( AddressT const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        ControlIdentity identity;
        lookupIdentityForAddress( identity, address );
        RangedValueBase const *v = m_target.getRangedValueForControlIdentity( identity, item_num, w_pos, h_pos );
        return v->getEncodingType();
    }

    EncodingType getStorageTypeForAddress( AddressT const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        ControlIdentity identity;
        lookupIdentityForAddress( identity, address );
        RangedValueBase const *v = m_target.getRangedValueForControlIdentity( identity, item_num, w_pos, h_pos );
        return v->getStorageType();
    }

    bool isStorageTypeString( AddressT const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        EncodingType t = getStorageTypeForAddress( address, item_num, w_pos, h_pos );

        bool r = false;
        if ( t == EncodingType::ENCODING_STRING64 || t == EncodingType::ENCODING_STRING406 )
        {
            r = true;
        }
        return r;
    }

    template <typename T>
    void getValue( T *value, AddressT const &address, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        ControlIdentity identity;
        lookupIdentityForAddress( identity, address );

        m_target.getValue( value, identity, item_num, w_pos, h_pos );
    }

    template <typename T>
    bool setValue( ControlIdentityComparatorPtr write_validator,
                   Milliseconds current_time_in_milliseconds,
                   T value,
                   AddressT const &address,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_target.getMutex() );
        try
        {
            ControlIdentity identity;
            lookupIdentityForAddress( identity, address );
            return m_target.setValue( write_validator, current_time_in_milliseconds, value, identity, item_num, w_pos, h_pos );
        }
        catch ( SchemaErrorReadOnly const &e )
        {
            throw SchemaAdaptorErrorReadOnly( e.m_identity, address );
        }
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
            throw SchemaAdaptorErrorAddressNotFound( address );
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
            throw SchemaAdaptorErrorAddressNotFound( address );
        }
        address = r;
    }

    class ChangeNotifierAdaptor : public ChangeNotifier
    {
      public:
        using Callback = std::function<void(Milliseconds, std::set<AddressT> const &)>;

        ChangeNotifierAdaptor( SchemaAdaptor &schema_adaptor ) : m_schema_adaptor( schema_adaptor ) {}

        virtual ~ChangeNotifierAdaptor() {}

        void removeAddressSubscription( AddressT const &address )
        {
            ControlIdentity identity;
            m_schema_adaptor.lookupIdentityForAddress( identity, address );
            ControlIdentityComparatorPtr range = std::make_shared<ControlIdentityComparatorUnique>( identity );

            removeSubscription( range );
        }

        void addAllSubscription( Milliseconds max_update_period_in_milliseconds,
                                 Milliseconds min_update_period_in_milliseconds,
                                 Callback callback )
        {
            ControlIdentityComparatorPtr range = std::make_shared<ControlIdentityComparatorAll>( m_schema_adaptor.getTarget() );

            addSubscription(
                range,
                max_update_period_in_milliseconds,
                min_update_period_in_milliseconds,
                [=]( Milliseconds cur_time, ControlIdentityComparatorPtr const &r, std::set<ControlIdentity> const &items )
                {
                    std::set<AddressT> address_items;
                    for ( auto &i : items )
                    {
                        AddressT a;
                        m_schema_adaptor.lookupAddressForIdentity( a, i );
                        address_items.insert( a );
                    }
                    callback( cur_time, address_items );
                } );
        }

        void addAddressSubscription( AddressT const &address,
                                     Milliseconds max_update_period_in_milliseconds,
                                     Milliseconds min_update_period_in_milliseconds,
                                     Milliseconds current_timestamp_in_milliseconds,
                                     Callback callback )
        {
            ControlIdentity identity;
            m_schema_adaptor.lookupIdentityForAddress( identity, address );
            ControlIdentityComparatorPtr range = std::make_shared<ControlIdentityComparatorUnique>( identity );

            addSubscription(
                range,
                max_update_period_in_milliseconds,
                min_update_period_in_milliseconds,
                current_timestamp_in_milliseconds,
                [=]( Milliseconds cur_time, ControlIdentityComparatorPtr const &r, std::set<ControlIdentity> const &items )
                {
                    std::set<AddressT> address_items;
                    for ( auto &i : items )
                    {
                        AddressT a;
                        m_schema_adaptor.lookupAddressForIdentity( a, i );
                        address_items.insert( a );
                    }
                    callback( cur_time, address_items );
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
      public:
        using ChangeNotifierAdaptorPtr = typename SchemaAdaptor<AddressT>::ChangeNotifierAdaptorPtr;
        ChangeNotifierHolder( SchemaAdaptor<AddressT> &target )
            : m_target( target ), m_notifier( target.createChangeNotifier() )
        {
        }

        ~ChangeNotifierHolder() { m_target.removeChangeNotifier( m_notifier ); }

        ChangeNotifierAdaptorPtr getNotifier() { return m_notifier; }

        ChangeNotifierManager &getManager() { return m_target.getChangeManager(); }

      private:
        SchemaAdaptor<AddressT> &m_target;
        ChangeNotifierAdaptorPtr m_notifier;
    };

  private:
    void collectItem( ControlIdentity const &identity, SchemaAddress const &schema_address ) override
    {
        AddressT address;
        getAddressForIdentity( address, identity, schema_address );
        m_address_map[address] = identity;
        m_identity_map[identity] = address;
    }

    std::map<AddressT, ControlIdentity> m_address_map;
    std::map<ControlIdentity, AddressT> m_identity_map;
};

void getAddressForIdentity( ControlIdentity &address, ControlIdentity const &identity, SchemaAddress const &schema_address );
}
