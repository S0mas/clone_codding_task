#include <simulator.hpp>
#include <device_controller.hpp>
#include "src/device_if.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Simulator e;
    DeviceIF deviceif;

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
