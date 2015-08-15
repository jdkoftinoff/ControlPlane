#include "ControlPlane/World.hpp"
#include "ControlPlane/ControlIdentityComparator.hpp"
#include "ControlPlane/Schema.hpp"

namespace ControlPlane
{

ControlIdentityComparatorUnique::ControlIdentityComparatorUnique( const ControlIdentity &identity ) : m_identity( identity ) {}

ControlIdentityComparatorUnique::~ControlIdentityComparatorUnique() {}

bool ControlIdentityComparatorUnique::containsControl( const ControlIdentity &identity ) const
{
    return identity == m_identity;
}

void ControlIdentityComparatorUnique::fillSet( std::set<ControlIdentity> &items ) const { items.insert( m_identity ); }

int ControlIdentityComparatorUnique::compare( const ControlIdentityComparator &other ) const
{
    int r = 0;

    if ( typeid( *this ) == typeid( other ) )
    {
        ControlIdentityComparatorUnique const &other_identity = static_cast<ControlIdentityComparatorUnique const &>( other );

        if ( m_identity == other_identity.m_identity )
        {
            r = 0;
        }
        else if ( m_identity < other_identity.m_identity )
        {
            r = -1;
        }
        else
        {
            r = 1;
        }
    }
    else if ( typeid( *this ).before( typeid( other ) ) )
    {
        r = -1;
    }
    else
    {
        r = 1;
    }
    return r;
}

void ControlIdentityComparatorUnique::print( std::ostream &o ) const
{
    o << "ChangeNotificationComparatorIdentity:" << m_identity << std::endl;
}

ControlIdentityComparatorAll::ControlIdentityComparatorAll( Schema const &schema ) : m_schema( schema ) {}

bool ControlIdentityComparatorAll::containsControl( const ControlIdentity & ) const { return true; }

void ControlIdentityComparatorAll::fillSet( std::set<ControlIdentity> &items ) const
{
    std::map<SchemaAddress, ControlIdentity> const &address_map = m_schema.getAddressMap();

    for ( auto const &i : address_map )
    {
        items.insert( i.second );
    }
}

int ControlIdentityComparatorAll::compare( const ControlIdentityComparator &other ) const
{
    int r = 0;

    if ( typeid( *this ) == typeid( other ) )
    {
        r = 0;
    }
    else if ( typeid( *this ).before( typeid( other ) ) )
    {
        r = -1;
    }
    else
    {
        r = 1;
    }
    return r;
}

void ControlIdentityComparatorAll::print( std::ostream &o ) const { o << "ChangeNotificationComparatorAll" << std::endl; }

ControlIdentityComparatorNone::ControlIdentityComparatorNone() {}

bool ControlIdentityComparatorNone::containsControl( const ControlIdentity & ) const { return false; }

void ControlIdentityComparatorNone::fillSet( std::set<ControlIdentity> &items ) const {}

int ControlIdentityComparatorNone::compare( const ControlIdentityComparator &other ) const
{
    int r = 1;
    if ( typeid( *this ) == typeid( other ) )
    {
        r = 0;
    }
    else if ( typeid( *this ).before( typeid( other ) ) )
    {
        r = -1;
    }
    return r;
}

void ControlIdentityComparatorNone::print( std::ostream &o ) const { o << "ControlIdentityComparatorNone"; }

ControlIdentityComparatorSet &ControlIdentityComparatorSet::operator=( const ControlIdentityComparatorSet &other )
{
    std::set<ControlIdentity> tmp;
    other.fillSet( tmp );
    {
        std::lock_guard<std::recursive_mutex> guard( m_mutex );
        m_items = tmp;
    }
    return *this;
}

size_t ControlIdentityComparatorSet::numItems() const
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    return m_items.size();
}

void ControlIdentityComparatorSet::addItem( const ControlIdentity &item )
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    m_items.insert( item );
}

void ControlIdentityComparatorSet::addItems( const std::set<ControlIdentity> &items )
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    for ( auto const &item : items )
    {
        m_items.insert( item );
    }
}

void ControlIdentityComparatorSet::removeItem( const ControlIdentity &item )
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    m_items.erase( item );
}

void ControlIdentityComparatorSet::removeItems( const std::set<ControlIdentity> &items )
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    for ( auto const &item : items )
    {
        m_items.erase( item );
    }
}

bool ControlIdentityComparatorSet::containsControl( const ControlIdentity &identity ) const
{
    bool r = false;
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    if ( m_items.find( identity ) != m_items.end() )
    {
        r = true;
    }
    return r;
}

void ControlIdentityComparatorSet::fillSet( std::set<ControlIdentity> &items ) const
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    for ( auto const &item : m_items )
    {
        items.insert( item );
    }
}

int ControlIdentityComparatorSet::compare( const ControlIdentityComparator &other_ ) const
{
    int r = 1;

    if ( typeid( other_ ) == typeid( *this ) )
    {
        ControlIdentityComparatorSet const &other = static_cast<ControlIdentityComparatorSet const &>( other_ );
        size_t my_size = numItems();
        size_t other_size = other.numItems();
        if ( my_size == other_size )
        {
            std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
            if ( m_items == other.m_items )
            {
                r = 0;
            }
            else if ( m_items < other.m_items )
            {
                r = -1;
            }
        }
        else
        {
            r = my_size - other_size;
        }
    }
    else
    {
        if ( typeid( *this ).before( typeid( other_ ) ) )
        {
            r = -1;
        }
    }

    return r;
}

void ControlIdentityComparatorSet::print( std::ostream &o ) const
{
    std::lock_guard<std::recursive_mutex> m_guard( m_mutex );
    for ( auto const &item : m_items )
    {
        o << item;
    }
}
}
