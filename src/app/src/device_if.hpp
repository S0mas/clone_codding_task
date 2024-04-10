#pragma once

#include <configuration.hpp>
#include <device_controller.hpp>
#include <measurement.hpp>
#include <parser.hpp>

#include <QObject>

class DeviceIF : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString lastResponse READ lastResponse NOTIFY lastResponseChanged)
    Q_PROPERTY(double pressure READ pressure NOTIFY lastMeasurementChanged)
    Q_PROPERTY(double temperature READ temperature NOTIFY lastMeasurementChanged)
    Q_PROPERTY(double velocity READ velocity NOTIFY lastMeasurementChanged)
    Q_PROPERTY(int frequency READ frequency NOTIFY configChanged)
    Q_PROPERTY(bool debug READ debug NOTIFY configChanged)

public:
    DeviceIF(DeviceController& controller, QObject* parent = nullptr);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void configure(int frequency, bool debug);

    auto lastResponse() const -> QString;
    auto setLastResponse(const QString &response) -> void;
    auto pressure() const -> double;
    auto temperature() const -> double;
    auto velocity() const -> double;
    auto setMeasurement(const Measurement& m) -> void;
    auto frequency() const -> int;
    auto debug() const -> bool;
    auto setConfig(const Configuration& c) -> void;

public:
signals:
    void lastResponseChanged();
    void lastMeasurementChanged();
    void configChanged();

private:
    DeviceController& controller_;
    QString lastResponse_;
    Measurement lastMeasurement_;
    Configuration config_;
};
