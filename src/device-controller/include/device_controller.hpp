#pragma once

#include <memory>
#include <functional>

class SerialTalker;

class DeviceController
{
public:
    using ReadCallback = std::function<void(std::string&&)>;

public:
    DeviceController();
    ~DeviceController();

    auto start() -> void;
    auto stop() -> void;
    auto configure(int frequency, bool debug) -> void;
    auto setReadCallback(const ReadCallback& callback) const -> void;
private:
    std::unique_ptr<SerialTalker> serialTalker_;
};
