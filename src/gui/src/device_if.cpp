#include "device_if.hpp"

DeviceIF::DeviceIF(DeviceController& controller, QObject* parent)
    : QObject{parent}
    , controller_{controller}
{}

auto DeviceIF::lastResponse() const -> QString
{
    return lastResponse_;
}

auto DeviceIF::setLastResponse(const QString &response) -> void
{
    lastResponse_ = response;
    emit lastResponseChanged();
}

auto DeviceIF::pressure() const -> double
{
    return lastMeasurement_.pressure_;
}

auto DeviceIF::temperature() const -> double
{
    return lastMeasurement_.temperature_;
}

auto DeviceIF::velocity() const -> double
{
    return lastMeasurement_.velocity_;
}

auto DeviceIF::setMeasurement(const Measurement& m) -> void
{
    if(m != lastMeasurement_)
    {
        lastMeasurement_ = m;
        emit lastMeasurementChanged();
    }
}

auto DeviceIF::frequency() const -> int
{
    return config_.frequency_;
}

auto DeviceIF::debug() const -> bool
{
    return config_.debug_;
}

auto DeviceIF::setConfig(const Configuration& c) -> void
{
    if(c != config_)
    {
        config_ = c;
        emit configChanged();
    }
}

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
