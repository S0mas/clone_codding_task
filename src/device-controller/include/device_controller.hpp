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
    auto configure(int frequency, bool debug) -> void;
    auto readResponse() const -> std::string;

private:
    std::unique_ptr<SerialTalker> serialTalker_;
};
