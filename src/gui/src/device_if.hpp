#pragma once

#include <device_controller.hpp>

#include <QObject>

class DeviceIF : public QObject
{
    Q_OBJECT

public:
    DeviceIF(QObject* parent = nullptr);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void configure(int frequency, bool debug);

private:
    DeviceController controller_;
};
