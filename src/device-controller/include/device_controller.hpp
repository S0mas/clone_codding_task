#pragma once

#include <memory>

class SerialTalker;

class DeviceController
{
public:
    DeviceController();
    ~DeviceController();

    auto start() -> void;
    auto stop() -> void;
    auto configure() -> void;
private:

private:
    std::unique_ptr<SerialTalker> serialTalker_;
};
