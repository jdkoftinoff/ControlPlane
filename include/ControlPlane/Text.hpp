#pragma once

#include "World.hpp"
#include <regex>

#include "SchemaAdaptor.hpp"

namespace ControlPlane
{
namespace Text
{
using std::string;

class TextIO
{
  public:
    virtual ~TextIO() {}

    virtual void sendLine( string const &line ) = 0;
    virtual bool receiveLine( string *result ) = 0;
};

struct TextAddress
{
    TextAddress() {}

    TextAddress( string const &other ) : m_value( other ) {}

    string m_value;

    inline friend bool operator==( TextAddress const &lhs, TextAddress const &rhs ) { return lhs.m_value == rhs.m_value; }

    inline friend bool operator<( TextAddress const &lhs, TextAddress const &rhs ) { return lhs.m_value < rhs.m_value; }

    inline friend std::ostream &operator<<( std::ostream &out, TextAddress const &rhs )
    {
        out << rhs.m_value;
        return out;
    }
};

using SchemaTextAdaptor = SchemaAdaptor<TextAddress>;

bool getTextAddressForIdentity( TextAddress &address,
                                ControlIdentity const &identity,
                                SchemaAddress const &schema_address,
                                Schema &schema );

struct TextProtocolSession
{
    TextIO &m_io;
    SchemaTextAdaptor &m_schema;
    SchemaTextAdaptor::ChangeNotifierHolder m_notifier;
    ControlIdentityComparatorPtr m_write_access;
    bool m_echo;

  public:
    using TextAddressSet = std::set<TextAddress>;

    TextProtocolSession( TextIO &io,
                         SchemaTextAdaptor &schema,
                         ControlIdentityComparatorPtr write_access = 0,
                         bool echo = false )
        : m_io( io ), m_schema( schema ), m_notifier( schema ), m_write_access( write_access ), m_echo( echo )
    {
        m_schema.collectDescriptors();
    }

    virtual ~TextProtocolSession() {}

    virtual bool handleLine( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleGet( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleSet( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleDescribe( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleSubscribe( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleUnsubscribe( Milliseconds current_time_in_milliseconds, string const &line );

    virtual void handleOtherCommands( Milliseconds current_time_in_milliseconds, string const &line );

  protected:
    virtual void handleIndividualGet( Milliseconds current_time_in_milliseconds, TextAddress const &address );

    virtual void handleIndividualSet( Milliseconds current_time_in_milliseconds, TextAddress const &address, string const &v );

    virtual void handleIndividualDescribe( Milliseconds current_time_in_milliseconds, TextAddress const &address );

    virtual void handleIndividualSubscribe( Milliseconds current_time_in_milliseconds,
                                            TextAddress const &address,
                                            Milliseconds min_time,
                                            Milliseconds max_time );

    virtual void handleIndividualUnsubscribe( Milliseconds current_time_in_milliseconds, TextAddress const &address );

    virtual string describeRangedValue( std::string const &name, const RangedValueBase &v );

  private:
    TextAddressSet getAddressesForRegex( string const &regex_string );

    bool doesContainRegex( string const &s );

    string normalizeAddress( string const &s );
};
}
}
