#include "message_processor.h"

#include <parser.hpp>

#include <QObject>
#include <QDebug>

auto MessageProcessor::processMessage(const std::string& msg) -> void
{
    qDebug() << __func__ << ": |" << msg.c_str() << "|";
    if(msg.empty())
    {
        return;
    }
    std::string errorReason;
    const auto& msgId = Parser::parseMessageId(msg, errorReason);
    if(!msgId.has_value())
    {
        emit invalidMessage(msg, errorReason);
        return;
    }

    switch (msgId.value())
    {
    case MessageId::START_TRANSMISSION:
        if(Parser::isValidateEmptyMsg(msg))
        {
            emit startTransmission();
        }
        else
        {
            emit invalidMessage(msg, "start transmission msg should not have any params");
        }

        break;
    case MessageId::STOP_TRANSMISSION:
        if(Parser::isValidateEmptyMsg(msg))
        {
            emit stopTransmission();
        }
        else
        {
            emit invalidMessage(msg, "stop transmission msg should not have any params");
        }
        break;
    case MessageId::SET_CONFIGURATION:
    {
        const auto& configuration = Parser::parseRequestConfig(msg);
        if(configuration.has_value())
        {
            emit setConfiguration(configuration.value());
        }
        else
        {
            emit invalidMessage(msg, "invalid configuration structure");
        }
        break;
    }
    case MessageId::INVALID:
        break;
    }
}


