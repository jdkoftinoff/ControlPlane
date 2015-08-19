#pragma once

#include "../World.hpp"
#include "Descriptors.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class Configuration : public DescriptorBase
{
  public:
    static const uint16_t descriptor_type = AVDECC_DESCRIPTOR_CONFIGURATION;

    Configuration( string description ) : DescriptorBase( description, descriptor_type ) {}

    void fillWriteAccess( ControlIdentityComparatorSetPtr &write_access ) override {}
    void storeToPDU( FixedBuffer &pdu ) const override;
};

template <typename... T>
ConfigurationPtr makeConfiguration( T &&... args )
{
    return ConfigurationPtr( new Configuration( args... ) );
}
}
}
