#include "device_controller.hpp"
#include <serial_talker.hpp>
#include <loguru.hpp>

#include <QProcessEnvironment>
#include <QString>

namespace
{

struct DeviceControllerSerialConfig
{
    DeviceControllerSerialConfig()
    {
        const int isSim = QProcessEnvironment::systemEnvironment().value("SIMULATION", "0").toInt();
        if(isSim == 0)
        {
            port = QProcessEnvironment::systemEnvironment().value("SERIAL_PORT", "/dev/ttyUSB0");
        }
        else
        {
            port = QProcessEnvironment::systemEnvironment().value("SIM_SERIAL_PORT_2", "/tmp/sim2");
        }
    }

    QString port;
};

}

DeviceController::DeviceController()
    : serialTalker_{std::make_unique<SerialTalker>(DeviceControllerSerialConfig().port)}
{
}

DeviceController::~DeviceController() = default;

auto DeviceController::start() -> void
{
    LOG_F(INFO, "Sending start transmission request to the device");
    serialTalker_->write("$0");
}

auto DeviceController::stop() -> void
{
    LOG_F(INFO, "Sending stop transmission request to the device");
    serialTalker_->write("$1");
}

auto DeviceController::configure(const Configuration& config) -> void
{
    LOG_F(INFO, "Sending new configuration to the device, frequency: %d, debug: %d" , config.frequency_, config.debug_);
    serialTalker_->write(QString("$2,%1,%2").arg(config.frequency_).arg(config.debug_).toStdString());
}

auto DeviceController::setReadCallback(const ReadCallback& callback) const -> void
{
    serialTalker_->setOnReadCallback(callback);
}
