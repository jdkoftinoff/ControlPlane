#pragma once

#include "World.hpp"
#include "RangedValue.hpp"
#include "Descriptors.hpp"
#include "ControlContainer.hpp"
#include "ChangeNotification.hpp"
#include "ChangeNotifierManager.hpp"

namespace ControlPlane
{

using Descriptor::DescriptorPtr;
using DescriptorAvdeccMap = Descriptor::DescriptorAvdeccMap;

inline std::string schemaAddressToString( SchemaAddress const &a )
{
    std::string r;
    for ( auto const &i : a )
    {
        r.append( i );
        r.append( " " );
    }
    return r;
}

class SchemaError : public std::runtime_error
{
  public:
    SchemaError( const std::string &e ) : std::runtime_error( e ) {}
};

class SchemaErrorNoSuchDescriptor : public SchemaError
{
  public:
    uint16_t m_descriptor_type;
    uint16_t m_descriptor_index;

    SchemaErrorNoSuchDescriptor( uint16_t descriptor_type, uint16_t descriptor_index )
        : SchemaError( Util::formstring( "SchemaErrorNoSuchDescriptor:", m_descriptor_type, ":", m_descriptor_index ) )
        , m_descriptor_type( descriptor_type )
        , m_descriptor_index( descriptor_index )
    {
    }
};

class SchemaErrorNoSuchControlIdentity : public SchemaError
{
  public:
    ControlIdentity m_identity;

    SchemaErrorNoSuchControlIdentity( ControlIdentity const &identity )
        : SchemaError( Util::formstring( "SchemaErrorNoSuchControlIdentity :", identity ) ), m_identity( identity )
    {
    }
};

class SchemaErrorReadOnly : public SchemaError
{
  public:
    ControlIdentity m_identity;

    SchemaErrorReadOnly( ControlIdentity const &identity )
        : SchemaError( Util::formstring( "SchemaErrorReadOnly :", identity ) ), m_identity( identity )
    {
    }
};

class SchemaErrorNoSuchDescriptorForAddress : public SchemaError
{
  public:
    SchemaAddress m_address;

    SchemaErrorNoSuchDescriptorForAddress( const SchemaAddress &address )
        : SchemaError( Util::formstring( "SchemaErrorNoSuchDescriptorForAddress:", schemaAddressToString( address ) ) )
        , m_address( address )
    {
    }
};

class Schema
{
    void collectDescriptors();

  public:
    Schema( ControlContainerPtr top_level ) : m_top_level( top_level ) { collectDescriptors(); }

    ControlIdentity getIdentityForAddress( SchemaAddress const &address ) const;

    DescriptorPtr const getDescriptor( ControlIdentity const &identity ) const;

    DescriptorPtr getDescriptor( ControlIdentity const &identity );

    const RangedValueBase *getRangedValueForControlIdentity( ControlIdentity const &identity,
                                                             int item_num = 0,
                                                             int w_pos = 0,
                                                             int h_pos = 0 ) const;

    RangedValueBase *getRangedValueForControlIdentity( ControlIdentityComparatorPtr write_validator,
                                                       ControlIdentity const &identity,
                                                       int item_num = 0,
                                                       int w_pos = 0,
                                                       int h_pos = 0 );

    template <typename T>
    bool setValue( ControlIdentityComparatorPtr write_validator,
                   Milliseconds current_time_in_milliseconds,
                   T value,
                   ControlIdentity const &identity,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        bool changed = false;
        RangedValueBase *ranged_value = getRangedValueForControlIdentity( write_validator, identity, item_num, w_pos, h_pos );
        changed = ranged_value->setUnencodedValue( value );
        if ( changed )
        {
            m_change_manager.controlChanged( current_time_in_milliseconds, identity );
        }
        return changed;
    }

    bool setValue( ControlIdentityComparatorPtr write_validator,
                   Milliseconds current_time_in_milliseconds,
                   std::string value,
                   ControlIdentity const &identity,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        bool changed = false;
        RangedValueBase *ranged_value = getRangedValueForControlIdentity( write_validator, identity, item_num, w_pos, h_pos );
        changed = ranged_value->setUnencodedValueString( value );
        if ( changed )
        {
            m_change_manager.controlChanged( current_time_in_milliseconds, identity );
        }
        return changed;
    }

    template <typename T>
    bool setValue( ControlIdentityComparatorPtr write_validator,
                   Milliseconds current_time_in_milliseconds,
                   T value,
                   SchemaAddress const &address,
                   int item_num = 0,
                   int w_pos = 0,
                   int h_pos = 0 )
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        return setValue(
            write_validator, current_time_in_milliseconds, value, getIdentityForAddress( address ), item_num, w_pos, h_pos );
    }

    template <typename T>
    void getValue( T *value, ControlIdentity const &identity, int item_num = 0, int w_pos = 0, int h_pos = 0 ) const
    {
        std::lock_guard<std::recursive_mutex> lock( m_access_mutex );
        RangedValueBase const *ranged_value = getRangedValueForControlIdentity( identity, item_num, w_pos, h_pos );
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

    std::map<SchemaAddress, ControlIdentity> const &getAddressMap() const { return m_address_map; }

  protected:
    ControlContainerPtr m_top_level;
    DescriptorAvdeccMap m_descriptor_avdecc_map;
    std::map<SchemaAddress, ControlIdentity> m_address_map;
    ChangeNotifierManager m_change_manager;
    mutable std::recursive_mutex m_access_mutex;
};
}
