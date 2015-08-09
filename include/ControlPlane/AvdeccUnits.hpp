#pragma once

#include "World.hpp"
#include "AvdeccUnits.hpp"

namespace ControlPlane
{

///
/// \brief The UnitsCode enum
///
/// Enumerations matching the Units definitions from IEEE Std 1722.1-2013 Clause 7.3.3
///
enum class UnitsCode : uint8_t
{
    Unitless = 0x00,
    Count = 0x01,
    Percent = 0x02,
    FStop = 0x03,
    TimeSeconds = 0x08,
    TimeMinutes = 0x09,
    TimeHours = 0x0a,
    TimeDays = 0x0b,
    TimeMonths = 0x0c,
    TimeYears = 0x0d,
    TimeSamples = 0x0e,
    TimeFrames = 0x0f,
    FrequencyHertz = 0x10,
    FrequencySemitones = 0x11,
    FrequencyCents = 0x12,
    FrequencyOctaves = 0x13,
    FrequencyFPS = 0x14,
    DistanceMetres = 0x18,
    TemperatureKelvin = 0x20,
    MassGrams = 0x28,
    VoltageVolts = 0x30,
    VoltageDBV = 0x31,
    VoltageDBU = 0x32,
    CurrentAMPS = 0x38,
    PowerWatts = 0x40,
    PowerDBM = 0x41,
    PowerDBW = 0x42,
    PressurePascals = 0x48,
    MemoryBits = 0x50,
    MemoryBytes = 0x51,
    MemoryKibiBytes = 0x52,
    MemoryMebiBytes = 0x53,
    MemoryGibiBytes = 0x54,
    MemoryTebiBytes = 0x55,
    MemoryBandwidthBitsPerSec = 0x58,
    MemoryBandwidthBytesPerSec = 0x59,
    MemoryBandwidthKibiBytesPerSec = 0x5a,
    MemoryBandwidthMebiBytesPerSec = 0x5b,
    MemoryBandwidthGigiBytesPerSec = 0x5c,
    MemoryBandwidthTebiBytesPerSec = 0x5d,
    LuminosityCandelas = 0x60,
    EnergyJoules = 0x68,
    AngleRadians = 0x70,
    ForceNewtons = 0x78,
    ResistanceOhms = 0x80,
    VelocityMetresPerSec = 0x88,
    VelocityRadiansPerSec = 0x89,
    AccelerationMetersPerSecSquared = 0x90,
    AccelerationRadiansPerSecSquared = 0x91,
    MagneticFluxTeslas = 0x98,
    AreaMetresSquared = 0xa0,
    VolumeMetersCubed = 0xa8,
    VolumeLitres = 0xa9,
    LevelDb = 0xb0,
    LevelDbPeak = 0xb1,
    LevelDbRms = 0xb2,
    LevelDbFs = 0xb3,
    LevelDbFsPeak = 0xb4,
    LevelDbFsRms = 0xb5,
    LevelDbTp = 0xb6,
    LevelDbA = 0xb7,
    LevelDbB = 0xb8,
    LevelDbC = 0xb9,
    LevelDbSpl = 0xba,
    LevelLU = 0xbb,
    LevelLUFS = 0xbc
};

///
/// \brief getAvdeccUnitsSuffix
///
/// Get description/suffix for a UnitsCode
///
/// \param units_code
/// \return UTF-8 c style string containing a presentable suffix describing the unit
///
const char *getAvdeccUnitsSuffix( UnitsCode units_code );
}
