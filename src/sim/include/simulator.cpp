#include "simulator.hpp"

#include "device.h"

#include <QThread>

Simulator::Simulator()
: worker_thread_{std::make_unique<QThread>()}
{
    device_ = new Device();
    device_->moveToThread(worker_thread_.get());

    QObject::connect(worker_thread_.get(), &QThread::started, device_, &Device::setup);
    QObject::connect(worker_thread_.get(), &QThread::finished, device_, &QObject::deleteLater);

    worker_thread_->start();
}

Simulator::~Simulator()
{
    worker_thread_->quit();
    worker_thread_->wait();
}
