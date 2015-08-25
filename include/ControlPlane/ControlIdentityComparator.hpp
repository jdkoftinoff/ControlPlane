#pragma once

#include "World.hpp"
#include "AvdeccSchema.hpp"
#include "ControlIdentity.hpp"

namespace ControlPlane
{

class ControlIdentityComparator;
using ControlIdentityComparatorPtr = std::shared_ptr<ControlIdentityComparator>;

class ControlIdentityComparator
{
  public:
    virtual ~ControlIdentityComparator() {}
    virtual bool containsControl( ControlIdentity const &identity ) const = 0;
    virtual void print( std::ostream &o ) const = 0;
    virtual void fillSet( std::set<ControlIdentity> &items ) const = 0;
    virtual int compare( ControlIdentityComparator const &other ) const = 0;
};

class ControlIdentityComparator_compare
{
  public:
    bool operator()( const std::shared_ptr<ControlIdentityComparator> &lhs,
                     const std::shared_ptr<ControlIdentityComparator> &rhs ) const
    {
        bool r = false;

        if ( lhs->compare( *rhs ) == -1 )
        {
            r = true;
        }
        return r;
    }
};

inline bool operator==( const ControlIdentityComparator &lhs, const ControlIdentityComparator &rhs )
{
    bool r = false;
    if ( lhs.compare( rhs ) == 0 )
    {
        r = true;
    }
    return r;
}

inline bool operator<( const ControlIdentityComparator &lhs, const ControlIdentityComparator &rhs )
{
    bool r = false;

    if ( lhs.compare( rhs ) < 0 )
    {
        r = true;
    }
    return r;
}

inline bool operator==( const ControlIdentityComparatorPtr &lhs, const ControlIdentityComparatorPtr &rhs )
{
    return *lhs == *rhs;
}

inline bool operator<( const ControlIdentityComparatorPtr &lhs, const ControlIdentityComparatorPtr &rhs )
{
    return *lhs < *rhs;
}

class ControlIdentityComparatorUnique : public ControlIdentityComparator
{
    ControlIdentity m_identity;

  public:
    ControlIdentityComparatorUnique( ControlIdentity const &identity );

    ~ControlIdentityComparatorUnique();

    bool containsControl( ControlIdentity const &identity ) const override;

    void fillSet( std::set<ControlIdentity> &items ) const override;

    int compare( ControlIdentityComparator const &other ) const override;

    void print( std::ostream &o ) const override;
};

class Schema;

class ControlIdentityComparatorNone : public ControlIdentityComparator
{
  public:
    ControlIdentityComparatorNone();

    bool containsControl( ControlIdentity const & ) const override;

    void fillSet( std::set<ControlIdentity> &items ) const override;

    int compare( ControlIdentityComparator const &other ) const override;

    void print( std::ostream &o ) const override;
};

class ControlIdentityComparatorAll : public ControlIdentityComparator
{
    Schema const &m_schema;

  public:
    ControlIdentityComparatorAll( Schema const &schema );

    bool containsControl( ControlIdentity const & ) const override;

    void fillSet( std::set<ControlIdentity> &items ) const override;

    int compare( ControlIdentityComparator const &other ) const override;

    void print( std::ostream &o ) const override;
};

class ControlIdentityComparatorSet : public ControlIdentityComparator
{
    mutable std::recursive_mutex m_mutex;
    std::set<ControlIdentity> m_items;

  public:
    ControlIdentityComparatorSet() {}

    ControlIdentityComparatorSet( ControlIdentityComparatorSet const &other ) { other.fillSet( m_items ); }

    ControlIdentityComparatorSet &operator=( ControlIdentityComparatorSet const &other );

    size_t numItems() const;

    void addItem( ControlIdentity const &item );

    void addItems( std::set<ControlIdentity> const &items );

    void removeItem( ControlIdentity const &item );

    void removeItems( std::set<ControlIdentity> const &items );

    bool containsControl( ControlIdentity const &identity ) const override;

    void fillSet( std::set<ControlIdentity> &items ) const override;

    int compare( ControlIdentityComparator const &other_ ) const override;

    void print( std::ostream &o ) const override;
};

using ControlIdentityComparatorSetPtr = std::shared_ptr<ControlIdentityComparatorSet>;

inline std::ostream &operator<<( std::ostream &o, ControlIdentityComparatorPtr const &v )
{
    v->print( o );
    return o;
}
}
