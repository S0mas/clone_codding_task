#pragma once

#include <memory>

class QThread;
class Device;

class Simulator
{
public:
    Simulator();
    ~Simulator();

private:
    std::unique_ptr<QThread> worker_thread_;
    Device* device_{nullptr};
};
