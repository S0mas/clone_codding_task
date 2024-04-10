#pragma once

#include <configuration.hpp>

#include <memory>
#include <functional>

class SerialTalker;

class DeviceController
{
public:
    using ReadCallback = std::function<void(const std::string&)>;

public:
    DeviceController();
    ~DeviceController();

    auto start() -> void;
    auto stop() -> void;
    auto configure(const Configuration& config) -> void;
    auto setReadCallback(const ReadCallback& callback) const -> void;
private:
    std::unique_ptr<SerialTalker> serialTalker_;
};
