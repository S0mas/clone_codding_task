#pragma once

#include "message_processor.h"
#include "device_configuration.h"
#include "state.h"

#include <QObject>

class QSerialPort;
class QTimer;

class Device : public QObject
{
    Q_OBJECT
public:
    Device(QObject* parent = nullptr);
    ~Device();

public slots:
    auto startTransmission() const -> void;
    auto stopTransmission() const -> void;
    auto setConfiguration(const DeviceConfiguration& newConfig) -> void;
    auto setup() -> void;
    auto sendState() const -> void;
private:
    auto openPort() -> void;
    auto closePort() -> void;
    auto read() const -> std::string;
    auto write(const std::string& msg) const -> void;
    auto setTimer(const float frequency) const -> void;
    auto reportError(const std::string& msg, const std::string& errorMsg) const -> void;
    auto responseSuccess(const std::string& msg) const -> void;
    auto responseFailure(const std::string& msg, const std::string& errorMsg) const -> void;
    auto response(const std::string& msg) const -> void;
private:
    QSerialPort* port_;
    State state_;
    DeviceConfiguration configuration_;
    MessageProcessor messageProcessor_;
    QTimer* messageSenderTimer_;
};
