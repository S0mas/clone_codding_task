#include "database.hpp"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

Database::Database()
{
    openConnection();
    createTables();
}

Database::~Database() = default;

auto Database::storeMeasurementMsg(const float pressure, const float temperature, const float velocity) const -> void
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO measurements (pressure, temperature, velocity) VALUES(%1, %2, %3)").arg(pressure).arg(temperature).arg(velocity));
    query.exec();
}

auto Database::storeResponseMsg(const std::string& msg) const -> void
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO responses (msg) VALUES('%1')").arg(QString::fromStdString(msg)));
    query.exec();
}

auto Database::replaceConfig(const int frequency, const bool debug) const -> void
{
    QSqlQuery query;
    query.prepare(QString("REPLACE INTO config (id, frequency, debug) VALUES(0, %1, %2)").arg(frequency).arg(debug));
    query.exec();
}

auto Database::openConnection() -> void
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/s0mas/clonebase.db");
    db.setHostName("s0mas");
    if(!db.open())
    {
        qDebug() << "dabase failure reason: " << db.lastError();
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
