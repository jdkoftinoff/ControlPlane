#include "ControlPlane/World.hpp"
#include "ControlPlane/ControlContainer.hpp"

namespace ControlPlane
{

ControlContainerPtr ControlContainer::addItem( const SchemaAddressElement &subnode )
{
    ControlContainerPtr item;

    // Return the existing container item if any, or create one if it doesn't exist

    auto container_it = m_container_items.find( subnode );
    if ( container_it != m_container_items.end() )
    {
        item = container_it->second;
    }
    else
    {
        item = ControlContainer::create();
        m_container_items[subnode] = item;
    }
    return item;
}

DescriptorPtr ControlContainer::addItem( const SchemaAddressElement &subnode, DescriptorPtr item )
{
    m_control_point_items[subnode] = std::make_pair( item, item->getControlIdentity() );
    return item;
}

DescriptorPtr ControlContainer::addItem( const SchemaAddressElement &subnode, DescriptorPtr item, ControlIdentity identity )
{
    m_control_point_items[subnode] = std::make_pair( item, identity );
    return item;
}

DescriptorPtr ControlContainer::addItem( const SchemaAddress &address, DescriptorPtr item, ControlIdentity identity )
{
    if ( address.size() == 1 )
    {
        addItem( address[0], item, identity );
    }
    else
    {
        SchemaAddress tmp;
        std::copy( address.begin() + 1, address.end(), std::back_inserter( tmp ) );

        if ( m_container_items.find( address[0] ) == m_container_items.end() )
        {
            addItem( address[0] );
        }

        m_container_items.at( address[0] ).get()->addItem( tmp, item, identity );
    }
    return item;
}

void ControlContainer::removeItem( const SchemaAddressElement &node )
{
    auto container_it = m_container_items.find( node );
    if ( container_it != m_container_items.end() )
    {
        m_container_items.erase( container_it );
    }
    auto control_it = m_control_point_items.find( node );
    if ( control_it != m_control_point_items.end() )
    {
        m_control_point_items.erase( control_it );
    }
}

DescriptorPtr ControlContainer::findDescriptor( const SchemaAddress &address, unsigned address_item )
{
    if ( address.size() == address_item + 1 )
    {
        return m_control_point_items.at( address.at( address_item ) ).first;
    }
    else
    {
        ControlContainerPtr container = m_container_items.at( address.at( address_item ) );
        return container->findDescriptor( address, address_item + 1 );
    }
}

ControlContainerPtr ControlContainer::findControlContainer( const SchemaAddress &address, unsigned address_item )
{
    if ( address.size() == address_item + 1 )
    {
        return m_container_items[address[address_item]];
    }
    else
    {
        ControlContainerPtr container = m_container_items[address[address_item]];
        return container->findControlContainer( address, address_item + 1 );
    }
}

void ControlContainer::updateControlIdentities()
{
    for ( auto item = m_container_items.begin(); item != m_container_items.end(); ++item )
    {
        item->second->updateControlIdentities();
    }

    for ( auto item = m_control_point_items.begin(); item != m_control_point_items.end(); ++item )
    {
        DescriptorPtr &descriptor = item->second.first;
        ControlIdentity &identity = item->second.second;

        if ( identity.m_descriptor_type != descriptor->getAvdeccDescriptorType()
             || identity.m_descriptor_index != descriptor->getAvdeccDescriptorIndex() )
        {
            identity.m_descriptor_type = descriptor->getAvdeccDescriptorType();
            identity.m_descriptor_index = descriptor->getAvdeccDescriptorIndex();
        }
    }
}

void ControlContainer::enumerate( std::function<void( const SchemaAddress &, DescriptorPtr, ControlIdentity )> callback )
{
    SchemaAddress working_address;
    enumerate( callback, working_address );
}

void
    ControlContainer::enumerate( std::function<void( const SchemaAddress &, DescriptorPtr, ControlIdentity identity )> callback,
                                 SchemaAddress &working_address )
{
    for ( auto item = m_container_items.begin(); item != m_container_items.end(); ++item )
    {
        working_address.push_back( item->first );
        item->second->enumerate( callback, working_address );
        working_address.pop_back();
    }

    for ( auto item = m_control_point_items.begin(); item != m_control_point_items.end(); ++item )
    {
        SchemaAddressElement const &address_element = item->first;
        DescriptorPtr &descriptor = item->second.first;
        ControlIdentity const &identity = item->second.second;
        working_address.push_back( address_element );
        callback( working_address, descriptor, identity );
        working_address.pop_back();
    }
}
}
