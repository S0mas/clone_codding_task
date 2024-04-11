#include "device_controller.hpp"
#include "serial_talker.hpp"

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
            port = QProcessEnvironment::systemEnvironment().value("SIM_SERIAL_PORT_2", "/home/sim2");
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
