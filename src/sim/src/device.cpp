#include "device.h"

#include <QDebug>
#include <QSerialPort>
#include <QTimer>

Q_DECLARE_METATYPE(DeviceConfiguration)

Device::Device(QObject* parent)
    : QObject(parent)
    , port_{nullptr}
    , messageSenderTimer_{nullptr}
{}

Device::~Device()
{
    port_->close();
}

auto Device::startTransmission() const -> void
{
    qDebug() << __func__;
    messageSenderTimer_->start();
}

auto Device::stopTransmission() const -> void
{
    qDebug() << __func__;
    messageSenderTimer_->stop();
}

auto Device::setConfiguration(const DeviceConfiguration& newConfig) -> void
{
    qDebug() << __func__;
    configuration_ = newConfig;
    setTimer(configuration_.frequency_);
}

auto Device::setup() -> void
{
    qDebug() << __func__;
    QObject::connect(&messageProcessor_, &MessageProcessor::startTransmission, this, &Device::startTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::stopTransmission, this, &Device::stopTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::setConfiguration, this, &Device::setConfiguration);
    QObject::connect(&messageProcessor_, &MessageProcessor::invalidMessageStructure, this, &Device::reportError);
    openPort();
}

auto Device::sendState() const -> void
{
    qDebug() << __func__;
    write(stateToString(state_) + "\n");
}

auto Device::openPort() -> void
{
    qDebug() << __func__;
    port_ = new QSerialPort(this);
    messageSenderTimer_ = new QTimer(this);
    QObject::connect(port_, &QIODevice::readyRead, this, [this](){ messageProcessor_.processMessage(read()); } );
    port_->setPortName("/home/s0mas/test");
    auto result = port_->open(QIODeviceBase::ReadWrite | QIODeviceBase::ExistingOnly);
    if(!result)
    {
        qDebug() << "Error: " << port_->errorString();
    }
    else
    {
        qDebug() << "Sucess! ";
    }
}

auto Device::closePort() -> void
{
    qDebug() << __func__;
    port_->close();
    port_->deleteLater();
    port_ = nullptr;
}

auto Device::read() const -> std::string
{
    QTextStream stream(port_);
    QString line;
    stream.readLineInto(&line);
    return line.toStdString();
}

auto Device::write(const std::string& msg) const -> void
{
    port_->write(msg.c_str());
    port_->flush();
}

auto Device::setTimer(const float frequency) const -> void
{
    const float sec = 1000.0; //ms
    messageSenderTimer_->setInterval(sec/frequency);
}

auto Device::reportError(const std::string& errorMsg) const -> void
{
    qDebug() << __func__ << ": " << errorMsg;
}
