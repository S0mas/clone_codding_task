#include <simulator.hpp>
#include <database.hpp>
#include <device_controller.hpp>
#include <server.hpp>
#include "src/device_if.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Database database;
    database.replaceConfig(99, false);
    database.storeMeasurementMsg(1, 2, 3);
    database.storeResponseMsg("testmsg");
    Simulator e;
    DeviceController controller;
    DeviceIF deviceif{controller};

    auto callback = [&database, &deviceif](const std::string& msg)
    {
        if(Parser::isResponse(msg))
        {
            database.storeResponseMsg(msg);
            deviceif.setLastResponse(QString::fromStdString(msg));

            if(Parser::isResponseValidConfig(msg))
            {
                const auto& config = Parser::parseResponseConfig(msg);
                if(config.has_value())
                {
                    database.replaceConfig(config.value());
                    deviceif.setConfig(config.value());
                }
            }
        }
        else
        {
            const auto& meas = Parser::parseMeasurement(msg);
            if(meas.has_value())
            {
                database.storeMeasurementMsg(meas.value());
                deviceif.setMeasurement(meas.value());
            }
        }
    };
    controller.setReadCallback(callback);

    Server server{controller};


    QVariantMap map;
    map.insert("deviceif", QVariant::fromValue(&deviceif));

    QQmlApplicationEngine engine;
    engine.setInitialProperties(map);

    const QUrl url(u"qrc:/mtg/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
