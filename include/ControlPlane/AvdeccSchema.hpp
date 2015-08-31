#pragma once

#include "World.hpp"
#include "Avdecc.hpp"
#include "SchemaAdaptor.hpp"

namespace ControlPlane
{

using SchemaAvdeccAdaptor = SchemaAdaptor<ControlIdentity>;

struct AvdeccSession
{
    SchemaAvdeccAdaptor &m_schema;
    SchemaAvdeccAdaptor::ChangeNotifierHolder m_notifier;
    ControlIdentityComparatorPtr m_write_access;

  public:
    AvdeccSession( SchemaAvdeccAdaptor &schema, ControlIdentityComparatorPtr write_access = 0 )
        : m_schema( schema ), m_notifier( schema ), m_write_access( write_access )
    {
    }

    virtual ~AvdeccSession() {}
};
}
