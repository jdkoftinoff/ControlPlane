#pragma once

#include "World.hpp"
#include "RangedValue.hpp"

namespace ControlPlane
{

using Mute = RangedValue<UnitsCode::Unitless, 0, 1, 0, 1, 0, uint8_t, bool>;

using Polarity = RangedValue<UnitsCode::Unitless, 0, 1, 0, 1, 0, uint8_t, bool>;

/** Gain
 *
 *
 * The gain value represents an audio gain in dB
 * from -90.0 dB to +10.0 dB inclusive
 * -90.0 dB also doubles as -inf.
 *
 */
using Gain = RangedValue<UnitsCode::LevelDb, -900, 100, 0, 10, -1, int32_t, float>;

/** AvdeccControlStringValue
 */
using ControlStringValue = RangedValue<UnitsCode::Unitless, 0, 0, 0, 0, 0, ControlPlane::AvdeccControlString, std::string>;

using VuMeterDbFsPeak = RangedValue<UnitsCode::LevelDbFsPeak, -128, 0, 0, 1, -1, int8_t, float>;

using DescriptorString = RangedValue<UnitsCode::Unitless, 0, 0, 0, 0, 0, ControlPlane::AvdeccNameString, std::string>;
}
