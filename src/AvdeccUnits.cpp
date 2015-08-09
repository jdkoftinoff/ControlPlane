#include "ControlPlane/World.hpp"
#include "ControlPlane/AvdeccUnits.hpp"

namespace ControlPlane
{

const char *getAvdeccUnitsSuffix( UnitsCode units_code )
{
    const char *s = "";
    switch ( units_code )
    {
    case UnitsCode::Unitless:
        s = "";
        break;
    case UnitsCode::Count:
        s = "";
        break;
    case UnitsCode::Percent:
        s = "%";
        break;
    case UnitsCode::FStop:
        s = "Fstop";
        break;
    case UnitsCode::TimeSeconds:
        s = "sec";
        break;
    case UnitsCode::TimeMinutes:
        s = "min";
        break;
    case UnitsCode::TimeHours:
        s = "hours";
        break;
    case UnitsCode::TimeDays:
        s = "days";
        break;
    case UnitsCode::TimeMonths:
        s = "months";
        break;
    case UnitsCode::TimeYears:
        s = "years";
        break;
    case UnitsCode::TimeSamples:
        s = "samples";
        break;
    case UnitsCode::TimeFrames:
        s = "frames";
        break;
    case UnitsCode::FrequencyHertz:
        s = "Hz";
        break;
    case UnitsCode::FrequencySemitones:
        s = "Semitones";
        break;
    case UnitsCode::FrequencyCents:
        s = "Cents";
        break;
    case UnitsCode::FrequencyOctaves:
        s = "Octaves";
        break;
    case UnitsCode::FrequencyFPS:
        s = "frames/sec";
        break;
    case UnitsCode::DistanceMetres:
        s = "metres";
        break;
    case UnitsCode::TemperatureKelvin:
        s = "ºK";
        break;
    case UnitsCode::MassGrams:
        s = "g";
        break;
    case UnitsCode::VoltageVolts:
        s = "V";
        break;
    case UnitsCode::VoltageDBV:
        s = "dBV";
        break;
    case UnitsCode::VoltageDBU:
        s = "dbU";
        break;
    case UnitsCode::CurrentAMPS:
        s = "A";
        break;
    case UnitsCode::PowerWatts:
        s = "W";
        break;
    case UnitsCode::PowerDBM:
        s = "dBM";
        break;
    case UnitsCode::PowerDBW:
        s = "dBW";
        break;
    case UnitsCode::PressurePascals:
        s = "Pa";
        break;
    case UnitsCode::MemoryBits:
        s = "bits";
        break;
    case UnitsCode::MemoryBytes:
        s = "Bytes";
        break;
    case UnitsCode::MemoryKibiBytes:
        s = "KiB";
        break;
    case UnitsCode::MemoryMebiBytes:
        s = "MiB";
        break;
    case UnitsCode::MemoryGibiBytes:
        s = "GiB";
        break;
    case UnitsCode::MemoryTebiBytes:
        s = "TiB";
        break;
    case UnitsCode::MemoryBandwidthBitsPerSec:
        s = "bps";
        break;
    case UnitsCode::MemoryBandwidthBytesPerSec:
        s = "Bps";
        break;
    case UnitsCode::MemoryBandwidthKibiBytesPerSec:
        s = "KiB/s";
        break;
    case UnitsCode::MemoryBandwidthMebiBytesPerSec:
        s = "MiB/s";
        break;
    case UnitsCode::MemoryBandwidthGigiBytesPerSec:
        s = "GiB/s";
        break;
    case UnitsCode::MemoryBandwidthTebiBytesPerSec:
        s = "TiB/s";
        break;
    case UnitsCode::LuminosityCandelas:
        s = "cd";
        break;
    case UnitsCode::EnergyJoules:
        s = "J";
        break;
    case UnitsCode::AngleRadians:
        s = "Rads";
        break;
    case UnitsCode::ForceNewtons:
        s = "Newtons";
        break;
    case UnitsCode::ResistanceOhms:
        s = "Ω";
        break;
    case UnitsCode::VelocityMetresPerSec:
        s = "m/s";
        break;
    case UnitsCode::VelocityRadiansPerSec:
        s = "rad/s";
        break;
    case UnitsCode::AccelerationMetersPerSecSquared:
        s = "m/s²";
        break;
    case UnitsCode::AccelerationRadiansPerSecSquared:
        s = "rad/s²";
        break;
    case UnitsCode::MagneticFluxTeslas:
        s = "T";
        break;
    case UnitsCode::AreaMetresSquared:
        s = "m²";
        break;
    case UnitsCode::VolumeMetersCubed:
        s = "m³";
        break;
    case UnitsCode::VolumeLitres:
        s = "L";
        break;
    case UnitsCode::LevelDb:
        s = "dB";
        break;
    case UnitsCode::LevelDbPeak:
        s = "dB (Peak)";
        break;
    case UnitsCode::LevelDbRms:
        s = "dB (RMS)";
        break;
    case UnitsCode::LevelDbFs:
        s = "dBFS";
        break;
    case UnitsCode::LevelDbFsPeak:
        s = "dBFS (Peak)";
        break;
    case UnitsCode::LevelDbFsRms:
        s = "dBFS (RMS)";
        break;
    case UnitsCode::LevelDbTp:
        s = "dBTP";
        break;
    case UnitsCode::LevelDbA:
        s = "dB (A)";
        break;
    case UnitsCode::LevelDbB:
        s = "dB (B)";
        break;
    case UnitsCode::LevelDbC:
        s = "dB (C)";
        break;
    case UnitsCode::LevelDbSpl:
        s = "dB (SPL)";
        break;
    case UnitsCode::LevelLU:
        s = "LU";
        break;
    case UnitsCode::LevelLUFS:
        s = "LUFS";
        break;
    default:
        s = "unknown";
    }
    return s;
}
}
