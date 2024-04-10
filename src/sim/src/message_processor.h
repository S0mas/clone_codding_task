#pragma once

#include <configuration.hpp>
#include <message_id.hpp>

#include <QObject>

class MessageProcessor : public QObject
{
    Q_OBJECT
public:
    auto processMessage(const std::string& msg) -> void;

public:
signals:
    void startTransmission();
    void stopTransmission();
    void setConfiguration(const Configuration& configuration);
    void invalidMessage(const std::string& msg, const std::string& errorMsg) const;
};
