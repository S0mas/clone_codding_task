#pragma once

#include "message_id.h"
#include "device_configuration.h"

#include <QObject>

class DeviceConfiguration;

class MessageProcessor : public QObject
{
    Q_OBJECT
public:
    auto processMessage(const std::string& msg) -> void;
private:
    auto validateEmptyMsg(const std::string& msg) const -> bool;
    auto parseMessageId(const std::string& msg, std::string& errorReason) const -> MessageId;


    auto parseConfiguration(const std::string& msg) const -> std::optional<DeviceConfiguration>;

public:
signals:
    void startTransmission();
    void stopTransmission();
    void setConfiguration(const DeviceConfiguration& configuration);
    void invalidMessage(const std::string& msg) const;
};
