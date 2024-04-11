#pragma once

#include <configuration.hpp>
#include <measurement.hpp>

#include <optional>
#include <string>
#include <vector>

class Database
{
public:
    Database();
    ~Database();

    auto storeMeasurementMsg(const Measurement& meas) const -> void;
    auto storeResponseMsg(const std::string& msg) const -> void;
    auto replaceConfig(const Configuration& config) const -> void;

    auto currentConfiguration() const -> std::optional<Configuration>;
    auto lastMeasurement() const -> std::optional<Measurement>;
    auto lastMeasurements(int max) const -> std::vector<Measurement>;
private:
    auto openConnection() -> void;
    auto createTables() -> void;
};

