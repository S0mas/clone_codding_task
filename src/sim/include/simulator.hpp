#pragma once

#include <memory>

class Device;

class Simulator
{
public:
    Simulator();
    ~Simulator();

private:
    std::unique_ptr<Device> device_;
};
