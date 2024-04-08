#include "device_controller.hpp"
#include "serial_talker.hpp"

#include <iostream>

DeviceController::DeviceController()
    : serialTalker_{std::make_unique<SerialTalker>("/home/s0mas/test2")}
{
    serialTalker_->setOnReadCallback([this](auto const& msg){ std::cout << "DeviceTalker received msg: " << msg << std::endl; });
    configure();
    start();
}

DeviceController::~DeviceController() = default;

auto DeviceController::start() -> void
{
    serialTalker_->write("$0");
}

auto DeviceController::stop() -> void
{
    serialTalker_->write("$1");
}

auto DeviceController::configure() -> void
{
    serialTalker_->write("$2,200,false");
}


