#pragma once

#include "device_controller.hpp"

#include <optional>

class Configuration;
class Database;
class Measurement;
class QHttpServer;
class QHttpServerResponse;

class Server
{
public:
    Server(DeviceController& controller, Database& database_);
    ~Server();

    auto setNewResponse(const std::string& msg) -> void;
private:
    auto takeResponse() -> std::string;
    auto waitForResponse(int ms = 100) -> void;
    auto sendResponse() -> QHttpServerResponse;

    auto lastMeasurement() -> std::optional<Measurement>;
    auto meanLast10Measurement() -> std::optional<Measurement>;
    auto currentConfiguration() -> std::optional<Configuration>;
private:
    DeviceController& controller_;
    Database& database_;
    std::unique_ptr<QHttpServer> server_;
    std::optional<std::string> response_;
};
