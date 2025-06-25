#pragma once
#include <pch.h>
#include <Types/Flags.h>
enum class AssetAvailability :uint8_t {
    eNONE = 0,
    eCPU = 1 << 1,
    eGPU = 1 << 2
};
using AssetAvailabilityFlags = uint8_t;
bool AssetAvailabilityInspector(Flags<AssetAvailability>& av);