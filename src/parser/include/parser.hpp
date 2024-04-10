#pragma once

#include "configuration.hpp"
#include "measurement.hpp"
#include "message_id.hpp"

#include <optional>
#include <string>

class Parser
{
public:
    static auto parseMeasurement(const std::string& msg) -> std::optional<Measurement>;
    static auto parseResponseConfig(const std::string& msg) -> std::optional<Configuration>;
    static auto parseRequestConfig(const std::string& msg) -> std::optional<Configuration>;
    static auto parseMessageId(const std::string& msg, std::string& errorReason) -> std::optional<MessageId>;
    static auto toMsg(const Measurement& meas) -> std::string;

    static auto isResponse(const std::string& msg) -> bool;
    static auto isResponseValidConfig(const std::string& msg) -> bool;
    static auto isRequestValidConfig(const std::string& msg) -> bool;
    static auto isValidateEmptyMsg(const std::string& msg) -> bool;
};
