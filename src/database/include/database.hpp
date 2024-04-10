#pragma once

#include <string>

class Database
{
public:
    Database();
    ~Database();

    auto storeMeasurementMsg(float pressure, float temperature, float velocity) const -> void;
    auto storeResponseMsg(const std::string& msg) const -> void;
    auto replaceConfig(int frequency, bool debug) const -> void;
private:
    auto openConnection() -> void;
    auto createTables() -> void;
};

