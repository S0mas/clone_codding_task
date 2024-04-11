#include "database.hpp"

#include <loguru.hpp>

#include <QProcessEnvironment>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace
{

struct DatabaseConfig
{
    DatabaseConfig()
    {
        databaseFileName = QProcessEnvironment::systemEnvironment().value("DATABASE_FILE_NAME", "database.db");
    }

    QString databaseFileName;
};

}

Database::Database()
{
    openConnection();
    createTables();
}

Database::~Database() = default;

auto Database::storeMeasurementMsg(const Measurement& meas) const -> void
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO measurements (pressure, temperature, velocity) VALUES(%1, %2, %3)").arg(meas.pressure_).arg(meas.temperature_).arg(meas.velocity_));
    query.exec();
}

auto Database::storeResponseMsg(const std::string& msg) const -> void
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO responses (msg) VALUES('%1')").arg(QString::fromStdString(msg)));
    const auto result = query.exec();

    if(result)
    {
        LOG_F(INFO, "Stored response from the device in database, response: %s", msg.c_str());
    }
    else
    {
        LOG_F(ERROR, "Failed to store response from the device in database, reason: %s", query.lastError().text().toStdString().c_str());
    }
}

auto Database::replaceConfig(const Configuration& config) const -> void
{
    QSqlQuery query;
    query.prepare(QString("REPLACE INTO config (id, frequency, debug) VALUES(0, %1, %2)").arg(config.frequency_).arg(config.debug_));
    const auto result = query.exec();

    if(result)
    {
        LOG_F(INFO, "Replaced device configuration in database, frequency: %d, debug: %d", config.frequency_, config.debug_);
    }
    else
    {
        LOG_F(ERROR, "Failed to replace device configuration in database, reason: %s", query.lastError().text().toStdString().c_str());
    }
}

auto Database::currentConfiguration() const -> std::optional<Configuration>
{
    QSqlQuery query;
    query.prepare("SELECT * FROM config");
    const auto result = query.exec();
    if(result && query.next())
    {
        return Configuration(query.value("frequency").toInt(), query.value("debug").toBool());
    }
    else
    {
        LOG_F(ERROR, "Failed to select currentConfiguration from database, reason: %s", query.lastError().text().toStdString().c_str());
    }
    return std::nullopt;
}

auto Database::lastMeasurement() const -> std::optional<Measurement>
{
    QSqlQuery query;
    query.prepare("SELECT * FROM measurements");
    const auto result = query.exec();
    if(result && query.last())
    {
        return Measurement(query.value("pressure").toDouble(), query.value("temperature").toDouble(), query.value("velocity").toDouble());
    }
    else
    {
        LOG_F(ERROR, "Failed to select last measurement from database, reason: %s", query.lastError().text().toStdString().c_str());
    }
    return std::nullopt;
}

auto Database::lastMeasurements(const int max) const -> std::vector<Measurement>
{
    QSqlQuery query;
    query.prepare("SELECT * FROM measurements");
    const auto result = query.exec() && query.last(); // pos to the last record
    std::vector<Measurement> measurements;
    int i = 0;
    auto isRecordAvailable = query.last();
    while(result && isRecordAvailable && i++ < max)
    {
        measurements.push_back({query.value("pressure").toDouble(), query.value("temperature").toDouble(), query.value("velocity").toDouble()});
        isRecordAvailable = query.previous();
    }
    return measurements;
}

auto Database::openConnection() -> void
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DatabaseConfig().databaseFileName);
    if(!db.open())
    {
        LOG_F(ERROR, "Failed to open database, path: %s reason: %s", DatabaseConfig().databaseFileName.toStdString().c_str(), db.lastError().text().toStdString().c_str());
    }
    {
        LOG_F(INFO, "Database opened successfully path: %s", DatabaseConfig().databaseFileName.toStdString().c_str());
    }
}

auto Database::createTables() -> void
{
    QSqlQuery config;
    config.prepare("CREATE TABLE IF NOT EXISTS config (id bit primary key, frequency tinyint not null, debug bit not null, timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP);");
    config.exec();

    QSqlQuery measurements;
    measurements.prepare("CREATE TABLE IF NOT EXISTS measurements (id int primary key, pressure float[16] not null, temperature float[16] not null, velocity float[16] not null, timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP);");
    measurements.exec();

    QSqlQuery responses;
    responses.prepare("CREATE TABLE IF NOT EXISTS responses (id int primary key, msg TINYTEXT not null, timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP);");
    responses.exec();
}
