#include "device_if.hpp"

DeviceIF::DeviceIF(DeviceController& controller, QObject* parent)
    : QObject{parent}
    , controller_{controller}
{}


auto DeviceIF::start() -> void
{
    controller_.start();
}

auto DeviceIF::stop() -> void
{
    controller_.stop();
}

auto DeviceIF::configure(const int frequency, const bool debug) -> void
{
    controller_.configure(frequency, debug);
}
