#include "simulator.hpp"

#include "device.h"

#include <QThread>

Simulator::Simulator()
{
    device_ = std::make_unique<Device>();

}

Simulator::~Simulator()
{

}
