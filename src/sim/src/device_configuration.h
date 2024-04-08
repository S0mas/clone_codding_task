#pragma once

#include <cstdint>

struct DeviceConfiguration
{
    std::uint8_t frequency_{10};
    bool debug_{false};
};
