#include "device.h"

#include "serial_talker.hpp"

#include <QDebug>
#include <QTimer>

Q_DECLARE_METATYPE(DeviceConfiguration)

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

auto Device::setConfiguration(const DeviceConfiguration& newConfig) -> void
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

    serialTalker_ = std::make_unique<SerialTalker>("/home/s0mas/test");
    messageSenderTimer_ = new QTimer(this);
    QObject::connect(messageSenderTimer_, &QTimer::timeout, this, &Device::sendState);
    serialTalker_->setOnReadCallback([this](auto const& msg)
                                     {
                                         qDebug() << "DeviceSim received msg: " << QString::fromStdString(msg);
                                         currentMsg_ = msg;
                                         messageProcessor_.processMessage(msg);
                                     });
}

auto Device::sendState() const -> void
{
    serialTalker_->write(stateToString(state_));
}

auto Device::setTimer(const float frequency) const -> void
{
    const float sec = 1000.0; //ms
    messageSenderTimer_->setInterval(sec/frequency);
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
