#include "device_if.hpp"

#include <simulator.hpp>
#include <database.hpp>
#include <device_controller.hpp>
#include <server.hpp>
#include <loguru.hpp>

#include <QGuiApplication>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace
{

struct AppConfig
{
    AppConfig()
    {
        no_gui = QProcessEnvironment::systemEnvironment().value("NO_GUI", "0");
        simulation = QProcessEnvironment::systemEnvironment().value("SIMULATION", "0");
    }

    bool runGui() const
    {
        return no_gui == "0";
    }

    bool runSimulation() const
    {
        return simulation == "1";
    }

    QString no_gui;
    QString simulation;
};

}

int main(int argc, char *argv[])
{
    std::unique_ptr<QCoreApplication> app;
    if(AppConfig().runGui())
    {
        LOG_F(INFO, "Starting with gui support");
        app = std::make_unique<QGuiApplication>(argc, argv);
    }
    else
    {
        app = std::make_unique<QCoreApplication>(argc, argv);
    }

    std::unique_ptr<Simulator> simulator;
    if(AppConfig().runSimulation())
    {
        LOG_F(INFO, "Starting the simulation");
        simulator = std::make_unique<Simulator>();
    }

    Database database;
    DeviceController controller;

    std::unique_ptr<DeviceIF> deviceif;
    if(AppConfig().runGui())
    {
        deviceif = std::make_unique<DeviceIF>(controller);
    }

    Server server{controller};

    auto callback = [&database, &deviceif, &server](const std::string& msg)
    {
        LOG_F(INFO, "Recived msg from the device %s", msg.c_str());
        if(Parser::isResponse(msg))
        {
            server.setNewResponse(msg);
            database.storeResponseMsg(msg);
            deviceif->setLastResponse(QString::fromStdString(msg));

            if(Parser::isResponseValidConfig(msg))
            {
                const auto& config = Parser::parseResponseConfig(msg);
                if(config.has_value())
                {
                    database.replaceConfig(config.value());
                    deviceif->setConfig(config.value());
                }
            }
        }
        else
        {
            const auto& meas = Parser::parseMeasurement(msg);
            if(meas.has_value())
            {
                database.storeMeasurementMsg(meas.value());
                deviceif->setMeasurement(meas.value());
            }
        }
    };
    controller.setReadCallback(callback);

    std::unique_ptr<QQmlApplicationEngine> engine;
    if(AppConfig().runGui())
    {
        QVariantMap map;
        map.insert("deviceif", QVariant::fromValue(deviceif.get()));

        engine = std::make_unique<QQmlApplicationEngine>() ;
        engine->setInitialProperties(map);

        const QUrl url(u"qrc:/clone/Main.qml"_qs);
        QObject::connect(engine.get(), &QQmlApplicationEngine::objectCreationFailed,
            app.get(), []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);

        engine->load(url);
    }

    return app->exec();
}
