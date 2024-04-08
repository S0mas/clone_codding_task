#pragma once

#include "message_processor.h"
#include "device_configuration.h"
#include "state.h"

#include <QObject>

class SerialTalker;
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
    auto setTimer(const float frequency) const -> void;
    auto reportError(const std::string& msg, const std::string& errorMsg) const -> void;
    auto responseSuccess(const std::string& msg) const -> void;
    auto responseFailure(const std::string& msg, const std::string& errorMsg) const -> void;
    auto response(const std::string& msg) const -> void;
private:
    State state_;
    DeviceConfiguration configuration_;
    MessageProcessor messageProcessor_;
    QTimer* messageSenderTimer_;
    std::string currentMsg_;
    std::unique_ptr<SerialTalker> serialTalker_;
};
