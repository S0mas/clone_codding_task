#pragma once

#include "device_controller.hpp"

#include <optional>

class QHttpServer;
class QHttpServerResponse;

class Server
{
public:
    Server(DeviceController& controller);
    ~Server();

    auto setNewResponse(const std::string& msg) -> void;
private:
    auto takeResponse() -> std::string;
    auto waitForResponse(int ms = 100) -> void;
    auto sendResponse() -> QHttpServerResponse;
private:
    DeviceController& controller_;
    std::unique_ptr<QHttpServer> server_;
    std::optional<std::string> response_;
};
