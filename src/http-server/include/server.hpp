#pragma once

#include "device_controller.hpp"

class QHttpServer;

class Server
{
public:
    Server(DeviceController& controller);
    ~Server();

private:
    DeviceController& controller_;
    std::unique_ptr<QHttpServer> server_;
};
