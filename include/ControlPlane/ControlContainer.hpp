#pragma once

#include "World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{

class ControlContainer;

using ControlContainerPtr = shared_ptr<ControlContainer>;
using ControlContainerMap = map<SchemaAddressElement, ControlContainerPtr>;
using DescriptorPtr = Descriptor::DescriptorPtr;
using DescriptorAddressMap = Descriptor::DescriptorAddressMap;

///
/// \brief The ControlContainer class
///
/// Holds one level of a hierarchy in the control point schema.
/// The ControlContainer object can contain more ControlContainer objects and
/// can contain ControlPoint objects.
///
class ControlContainer
{
  public:
    static ControlContainerPtr create() { return make_shared<ControlContainer>(); }

    ///
    /// \brief ControlContainer
    ///
    /// Constructor for the ControlContainer object
    ///
    ControlContainer() {}

    ///
    /// \brief getContainerItems
    ///
    /// Get the map of ControlContainers that are held in this object.
    /// The key for each entry in the map is the AddressComponent, and the
    /// value for each entry is a shared_ptr to the ControlContainer
    /// \return const ControlContainerMap reference
    ///
    const ControlContainerMap &getContainerItems() const { return m_container_items; }

    ///
    /// \brief getContainerItems
    ///
    /// Get the map of ControlContainers that are held in this object.
    /// The key for each entry in the map is the AddressComponent, and the
    /// value for each entry is a shared_ptr to the ControlContainer
    /// \return ControlContainerMap reference
    ///
    ControlContainerMap &getContainerItems() { return m_container_items; }

    ///
    /// \brief addItem
    ///
    /// Add a ControlContainer associated with an AddressElement
    ///
    /// \param subnode the AddressElement
    /// \return ControlContainer
    ///
    ControlContainerPtr addItem( const SchemaAddressElement &subnode );

    ///
    /// \brief addItem
    /// \param subnode
    /// \param info
    /// \param ranged_value
    /// \return
    ///
    DescriptorPtr addItem( const SchemaAddressElement &subnode, DescriptorPtr info );

    DescriptorPtr addItem( const SchemaAddressElement &subnode, DescriptorPtr item, ControlIdentity identity );

    DescriptorPtr addItem( const SchemaAddress &address, DescriptorPtr item, ControlIdentity identity );

    ///
    /// \brief removeItem
    ///
    /// Remove any items that are associated with the AddressElement
    ///
    /// \param node AddressElement to remove
    ///
    void removeItem( const SchemaAddressElement &node );

    ///
    /// \brief findControlPoint
    ///
    /// Recursively find the ControlPoint in this container associated with an Address, starting at address_item level in the
    /// hierarchy
    /// \param address The Address to find
    /// \param address_item The level in the Address hierarchy to search at
    /// \return
    ///
    DescriptorPtr findDescriptor( const SchemaAddress &address, unsigned address_item = 0 );

    ///
    /// \brief findControlContainer
    /// \param address
    /// \param address_Item
    /// \return
    ///
    ControlContainerPtr findControlContainer( const SchemaAddress &address, unsigned address_Item = 0 );

    ///
    /// \brief getControlPointItems
    ///
    /// Get the map of ControlPoints that are held in this object.
    /// The key for each entry in the map is the AddressComponent, and the
    /// value for each entry is a shared_ptr to the ControlPoint
    /// \return const ControlContainerMap reference
    ///
    const DescriptorAddressMap &getControlPointItems() const { return m_control_point_items; }

    ///
    /// \brief getControlPointItems
    ///
    /// Get the map of ControlPoints that are held in this object.
    /// The key for each entry in the map is the AddressComponent, and the
    /// value for each entry is a shared_ptr to the ControlPoint
    /// \return ControlContainerMap reference
    ///
    DescriptorAddressMap &getControlPointItems() { return m_control_point_items; }

    void updateControlIdentities();

    void enumerate(
        std::function<void( const SchemaAddress &address, DescriptorPtr descriptor, ControlIdentity identity )> callback );

    void enumerate(
        std::function<void( const SchemaAddress &address, DescriptorPtr descriptor, ControlIdentity identity )> callback,
        SchemaAddress &working_address );

  private:
    ControlContainerMap m_container_items;
    DescriptorAddressMap m_control_point_items;
};
}
