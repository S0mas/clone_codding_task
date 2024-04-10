#pragma once

#include <configuration.hpp>
#include <measurement.hpp>

#include <string>

class Database
{
public:
    Database();
    ~Database();

    auto storeMeasurementMsg(const Measurement& meas) const -> void;
    auto storeResponseMsg(const std::string& msg) const -> void;
    auto replaceConfig(const Configuration& config) const -> void;
private:
    auto openConnection() -> void;
    auto createTables() -> void;
};

