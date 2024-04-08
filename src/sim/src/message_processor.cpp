#include "message_processor.h"

#pragma once

#include <QObject>
#include <QDebug>

#include <sstream>

std::istringstream& operator>>(std::istringstream& str, uint8_t& num) {
    int temp;
    str >> temp;
    constexpr auto max = std::numeric_limits<uint8_t>::max();
    if(temp >= 0 && temp <= max)
    {
        num = temp;
    }
    else
    {
        str.setstate(std::ios::failbit);
    }
    return str;
}

auto toUint8(const std::string& token) -> std::optional<uint8_t>
{
    std::istringstream valuesreader{token};
    uint8_t value;
    valuesreader >> value;
    return valuesreader.fail() || !valuesreader.eof() ? std::nullopt : std::optional<uint8_t>{value};
}

auto toBool(const std::string& token) -> std::optional<bool>
{
    std::istringstream valuesreader{token};
    bool value;
    valuesreader >> value;
    if (valuesreader.fail())
    {
        if(token == "true" || token == "TRUE")
        {
            return true;
        }
        if(token == "false" || token == "FALSE")
        {
            return false;
        }
        return std::nullopt;
    }
    return !valuesreader.eof() ? std::nullopt : std::optional<bool>{value};
}

auto MessageProcessor::processMessage(const std::string& msg) -> void
{
    qDebug() << __func__ << ": |" << msg.c_str() << "|";
    if(msg.empty())
    {
        return;
    }
    std::string errorReason;
    const auto& msgId = parseMessageId(msg, errorReason);
    switch (msgId)
    {
    case MessageId::START_TRANSMISSION:
        if(validateEmptyMsg(msg))
        {
            emit startTransmission();
        }
        else
        {
            emit invalidMessage("start transmission msg should not have any params");
        }

        break;
    case MessageId::STOP_TRANSMISSION:
        if(validateEmptyMsg(msg))
        {
            emit stopTransmission();
        }
        else
        {
            emit invalidMessage("stop transmission msg should not have any params");
        }
        break;
    case MessageId::SET_CONFIGURATION:
    {
        const auto& configuration = parseConfiguration(msg);
        if(configuration.has_value())
        {
            emit setConfiguration(configuration.value());
        }
        else
        {
            emit invalidMessage("invalid configuration structure");
        }
        break;
    }
    case MessageId::INVALID:
        break;
    }
}

auto MessageProcessor::validateEmptyMsg(const std::string& msg) const -> bool
{
    return msg.size() == 2;
}
auto MessageProcessor::parseMessageId(const std::string& msg, std::string& errorReason) const -> MessageId
{
    const auto msgStartSymbol = '$';
    std::istringstream inpstream{msg};
    if(inpstream.get() != msgStartSymbol)
    {
        errorReason = "msg should start with '$'";
        return MessageId::INVALID;
    }

    int id;
    inpstream >> id;
    if(inpstream.fail())
    {
        errorReason = "msg id is invalid";
        return MessageId::INVALID;
    }

    if(id == 0)
    {
        qDebug() << __func__ << ": start transmition id";
        return MessageId::START_TRANSMISSION;
    }
    if(id == 1)
    {
        qDebug() << __func__ << ": stop transmition id";
        return MessageId::STOP_TRANSMISSION;
    }
    if(id == 2)
    {
        return MessageId::SET_CONFIGURATION;
    }
    errorReason = "msg id is unknown";
    return MessageId::INVALID;
}


auto MessageProcessor::parseConfiguration(const std::string& msg) const -> std::optional<DeviceConfiguration>
{
    qDebug() << __func__;
    std::istringstream iss{msg};
    std::string token;
    std::vector<uint8_t> values;
    std::getline(iss, token, ','); // skipFirstToken with message id '$X,'

    DeviceConfiguration conf;
    std::getline(iss, token, ','); // frequency;

    if(const auto& value = toUint8(token); value.has_value())
    {
        qDebug() << "Succes! value: " << *value;
        conf.frequency_ = value.value();
    }
    else
    {
        qDebug() << "Fail! token: " << token;
        // invalid value
        return std::nullopt;
    }

    std::getline(iss, token, ','); // debug;
    if(const auto& value = toBool(token); value.has_value())
    {
        qDebug() << "Succes! value: " << *value;
        conf.debug_ = value.value();
    }
    else
    {
        qDebug() << "Fail! token: " << token;
        // invalid value
        return std::nullopt;
    }

    if(!iss.eof())
    {
        // too many values
        return std::nullopt;
    }

    return conf;
}

