#pragma once

#include "../World.hpp"
#include "../Values.hpp"
#include "../AvdeccSchema.hpp"

#include "Descriptor.hpp"
#include "DescriptorCounts.hpp"

namespace ControlPlane
{
namespace Descriptor
{

class AudioCluster;
class AudioMap;
class AudioUnit;
class AvbInterface;
class ClockDomain;
class ClockSource;
class Configuration;
class Control;
class ControlBlock;
class Entity;
class ExternalPort;
class ExternalPortInput;
class ExternalPortOutput;
class InternalPort;
class InternalPortInput;
class InternalPortOutput;
class Jack;
class JackInput;
class JackOutput;
class Locale;
class Matrix;
class MatrixSignal;
class MemoryObject;
class Mixer;
class SignalSelector;
class Stream;
class StreamInput;
class StreamOutput;
class StreamPort;
class StreamPortInput;
class StreamPortOutput;
class Strings;

using AudioClusterPtr = std::shared_ptr<AudioCluster>;
using AudioMapPtr = std::shared_ptr<AudioMap>;
using AudioUnitPtr = std::shared_ptr<AudioUnit>;
using AvbInterfacePtr = std::shared_ptr<AvbInterface>;
using ClockDomainPtr = std::shared_ptr<ClockDomain>;
using ClockSourcePtr = std::shared_ptr<ClockSource>;
using ConfigurationPtr = std::shared_ptr<Configuration>;
using ControlPtr = std::shared_ptr<Control>;
using ControlBlockPtr = std::shared_ptr<ControlBlock>;
using EntityPtr = std::shared_ptr<Entity>;
using ExternalPortPtr = std::shared_ptr<ExternalPort>;
using ExternalPortInputPtr = std::shared_ptr<ExternalPortInput>;
using ExternalPortOutputPtr = std::shared_ptr<ExternalPortOutput>;
using InternalPortPtr = std::shared_ptr<InternalPort>;
using InternalPortInputPtr = std::shared_ptr<InternalPortInput>;
using InternalPortOutputPtr = std::shared_ptr<InternalPortOutput>;
using JackPtr = std::shared_ptr<Jack>;
using JackInputPtr = std::shared_ptr<JackInput>;
using JackOutputPtr = std::shared_ptr<JackOutput>;
using LocalePtr = std::shared_ptr<Locale>;
using MatrixPtr = std::shared_ptr<Matrix>;
using MatrixSignalPtr = std::shared_ptr<MatrixSignal>;
using MemoryObjectPtr = std::shared_ptr<MemoryObject>;
using MixerPtr = std::shared_ptr<Mixer>;
using SignalSelectorPtr = std::shared_ptr<SignalSelector>;
using StreamPtr = std::shared_ptr<Stream>;
using StreamInputPtr = std::shared_ptr<StreamInput>;
using StreamOutputPtr = std::shared_ptr<StreamOutput>;
using StreamPortPtr = std::shared_ptr<StreamPort>;
using StreamPortInputPtr = std::shared_ptr<StreamPortInput>;
using StreamPortOutputPtr = std::shared_ptr<StreamPortOutput>;
using StringsPtr = std::shared_ptr<Strings>;
}
}
