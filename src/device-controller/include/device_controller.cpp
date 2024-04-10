#include "device_controller.hpp"
#include "serial_talker.hpp"

#include <QString>

DeviceController::DeviceController()
    : serialTalker_{std::make_unique<SerialTalker>("/home/s0mas/test2")}
{
}

DeviceController::~DeviceController() = default;

auto DeviceController::start() -> void
{
    serialTalker_->write("$0");
}

auto DeviceController::stop() -> void
{
    serialTalker_->write("$1");
}

auto DeviceController::configure(const Configuration& config) -> void
{
    serialTalker_->write(QString("$2,%1,%2").arg(config.frequency_).arg(config.debug_).toStdString());
}

auto DeviceController::setReadCallback(const ReadCallback& callback) const -> void
{
    serialTalker_->setOnReadCallback(callback);
}
