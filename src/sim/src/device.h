#pragma once

#include "message_processor.h"

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
    auto setConfiguration(const Configuration& newConfig) -> void;
    auto setup() -> void;
    auto sendMeasurement() const -> void;
private:
    auto setTimer(const double frequency) const -> void;
    auto reportError(const std::string& msg, const std::string& errorMsg) const -> void;
    auto responseSuccess(const std::string& msg) const -> void;
    auto responseFailure(const std::string& msg, const std::string& errorMsg) const -> void;
    auto response(const std::string& msg) const -> void;
private:
    Configuration configuration_;
    MessageProcessor messageProcessor_;
    QTimer* messageSenderTimer_;
    std::string currentMsg_;
    std::unique_ptr<SerialTalker> serialTalker_;
};
