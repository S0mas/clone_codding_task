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
    auto parseMessageId(const std::string& msg) const -> MessageId;


    auto parseConfiguration(const std::string& msg) const -> std::optional<DeviceConfiguration>;

public:
signals:
    void startTransmition();
    void stopTransmition();
    void setConfiguration(const DeviceConfiguration& configuration);
    void invalidMessageStructure(const std::string& msg = "invalid message structure") const;
    void unknownMessage(const std::string& msg = "unknown message id") const;
};
