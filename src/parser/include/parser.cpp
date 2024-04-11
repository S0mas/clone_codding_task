#include "parser.hpp"

#include <loguru.hpp>

#include <cstdint>
#include <limits>
#include <sstream>

auto toBool(const std::string& token) -> std::optional<bool>
{
    std::istringstream valuesreader{token};
    bool value;
    valuesreader >> value;
    if(valuesreader.fail())
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

auto operator>>(std::istringstream& str, uint8_t& num) -> std::istringstream&
{
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

auto Parser::parseMeasurement(const std::string& msg) -> std::optional<Measurement>
{
    std::string token;
    std::istringstream iss{msg};

    std::getline(iss, token, ',');
    token.erase(0, 1); // remove '$'

    Measurement m;
    m.pressure_ = std::stod(token);

    std::getline(iss, token, ',');
    m.temperature_ = std::stod(token);

    std::getline(iss, token, ',');
    m.velocity_ = std::stod(token);

    if(iss.fail())
    {
        LOG_F(ERROR, "Failed to parse msg as measurement, msg: %s", msg.c_str());
        return std::nullopt;
    }
    return m;
}

auto Parser::parseConfigFromTokens(const std::string& frequencyToken, const std::string& debugToken) -> std::optional<Configuration>
{
    Configuration c;
    if(const auto& value = toUint8(frequencyToken); value.has_value())
    {
        c.frequency_ = value.value();
    }
    else
    {
        LOG_F(ERROR, "Failed to parse token as frequency(uint8), token: %s", frequencyToken.c_str());
        return std::nullopt;
    }

    if(const auto& value = toBool(debugToken); value.has_value())
    {
        c.debug_ = value.value();
    }
    else
    {
        LOG_F(ERROR, "Failed to parse token as debug(bool), token: %s", debugToken.c_str());
        return std::nullopt;
    }
    return c;
}

auto Parser::parseResponseConfig(const std::string& msg) -> std::optional<Configuration>
{
    std::istringstream iss{msg};
    std::string token;
    std::getline(iss, token, ','); // skipFirstToken with message id '$X,'


    std::string frequency;
    std::string debug;
    std::getline(iss, frequency, ',');
    std::getline(iss, debug, ',');

    return parseConfigFromTokens(frequency, debug);
}

auto Parser::parseRequestConfig(const std::string& msg) -> std::optional<Configuration>
{
    std::istringstream iss{msg};
    std::string token;
    std::getline(iss, token, ','); // skipFirstToken with message id '$X,'


    std::string frequency;
    std::string debug;
    std::getline(iss, frequency, ',');
    std::getline(iss, debug, ',');
    if(!iss.eof())
    {
        LOG_F(ERROR, "Failed to parse msg as configuration, msg: %s", msg.c_str());
        return std::nullopt;
    }

    return parseConfigFromTokens(frequency, debug);
}

auto Parser::parseMessageId(const std::string& msg, std::string& errorReason) -> std::optional<MessageId>
{
    const auto msgStartSymbol = '$';
    std::istringstream inpstream{msg};
    if(inpstream.get() != msgStartSymbol)
    {
        errorReason = "msg should start with '$'";
        LOG_F(ERROR, "Failed to parse msg, msg: %s, reason: %s", msg.c_str(), errorReason.c_str());
        return std::nullopt;
    }

    int id;
    inpstream >> id;
    if(inpstream.fail())
    {
        errorReason = "msg id is invalid";
        LOG_F(ERROR, "Failed to parse msg, msg: %s, reason: %s", msg.c_str(), errorReason.c_str());
        return MessageId::INVALID;
    }

    if(id == 0)
    {
        return MessageId::START_TRANSMISSION;
    }
    if(id == 1)
    {
        return MessageId::STOP_TRANSMISSION;
    }
    if(id == 2)
    {
        return MessageId::SET_CONFIGURATION;
    }
    errorReason = "msg id is unknown";
    LOG_F(ERROR, "Failed to parse msg, msg: %s, reason: %s", msg.c_str(), errorReason.c_str());
    return std::nullopt;
}

auto Parser::toMsg(const Measurement& meas) -> std::string
{
    std::stringstream stream;
    stream << '$';
    stream << meas.pressure_;
    stream << ',';
    stream << meas.temperature_;
    stream << ',';
    stream << meas.velocity_;
    return stream.str();
}

auto Parser::isResponse(const std::string& msg) -> bool
{
    return msg.starts_with("$0,") || msg.starts_with("$1,") || msg.starts_with("$2,");
}

auto Parser::isResponseValidConfig(const std::string& msg) -> bool
{
    return msg.starts_with("$2,") && msg.ends_with(",ok");
}

auto Parser::isValidateEmptyMsg(const std::string& msg) -> bool
{
    return msg.size() == 2;
}
