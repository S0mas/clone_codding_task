#include "device.h"

#include <measurement.hpp>
#include <parser.hpp>
#include <serial_talker.hpp>

#include <QDebug>
#include <QProcessEnvironment>
#include <QTimer>

Q_DECLARE_METATYPE(Configuration)

namespace
{

struct DeviceSerialConfig
{
    DeviceSerialConfig()
    {
        port = QProcessEnvironment::systemEnvironment().value("SIM_SERIAL_PORT_1", "/home/s0mas/sim1");
    }

    QString port;
};

}

Device::Device(QObject* parent)
    : QObject(parent)
    , messageSenderTimer_{nullptr}
{
    setup();
}

Device::~Device() = default;

auto Device::startTransmission() const -> void
{
    qDebug() << __func__;
    messageSenderTimer_->start();
    responseSuccess(currentMsg_);
}

auto Device::stopTransmission() const -> void
{
    qDebug() << __func__;
    messageSenderTimer_->stop();
    responseSuccess(currentMsg_);
}

auto Device::setConfiguration(const Configuration& newConfig) -> void
{
    configuration_ = newConfig;
    setTimer(configuration_.frequency_);
    responseSuccess(currentMsg_);
}

auto Device::setup() -> void
{
    qDebug() << __func__;
    QObject::connect(&messageProcessor_, &MessageProcessor::startTransmission, this, &Device::startTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::stopTransmission, this, &Device::stopTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::setConfiguration, this, &Device::setConfiguration);
    QObject::connect(&messageProcessor_, &MessageProcessor::invalidMessage, this, &Device::reportError);
    QObject::connect(&messageProcessor_, &MessageProcessor::invalidMessage, this, &Device::responseFailure);

    serialTalker_ = std::make_unique<SerialTalker>(DeviceSerialConfig().port);
    messageSenderTimer_ = new QTimer(this);
    QObject::connect(messageSenderTimer_, &QTimer::timeout, this, &Device::sendMeasurement);
    serialTalker_->setOnReadCallback([this](auto const& msg)
                                     {
                                         qDebug() << "DeviceSim received msg: " << QString::fromStdString(msg);
                                         currentMsg_ = msg;
                                         messageProcessor_.processMessage(msg);
                                     });
}

auto Device::sendMeasurement() const -> void
{
    Measurement meas;
    meas.pressure_ = 111.22;
    meas.temperature_ = 1341.9;
    meas.velocity_ = 888.123;
    serialTalker_->write(Parser::toMsg(meas));
}

auto Device::setTimer(const double frequency) const -> void
{
    if(frequency == 0)
    {
        messageSenderTimer_->setInterval(std::numeric_limits<int>::max());
    }
    else
    {
        const auto sec = 1000.0; //ms
        messageSenderTimer_->setInterval(sec/frequency);
    }
}

auto Device::reportError(const std::string& msg, const std::string& errorMsg) const -> void
{
    qDebug() << __func__ << ": " << errorMsg;
}

auto Device::responseSuccess(const std::string& msg) const -> void
{
    response(msg + ",ok");
}

auto Device::responseFailure(const std::string& msg, const std::string& errorMsg) const -> void
{
    response(msg + "," + errorMsg);
}

auto Device::response(const std::string& msg) const -> void
{
    qDebug() << "Device::response " << msg;
    serialTalker_->write(msg);
}
