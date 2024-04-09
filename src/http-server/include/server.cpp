#include "server.hpp"

#include <QtHttpServer>

Server::Server(DeviceController& controller)
    : controller_{controller}
    , server_{std::make_unique<QHttpServer>()}
{

    server_->route("/start", QHttpServerRequest::Method::Get,
                     [&controller](const QHttpServerRequest &request) {
                         controller.start();
                         return QHttpServerResponse(QString::fromStdString(controller.readResponse()));
                     }
                     );

    server_->route("/stop", QHttpServerRequest::Method::Get,
                     [&controller](const QHttpServerRequest &request) {
                         controller.stop();
                         return QHttpServerResponse(QString::fromStdString(controller.readResponse()));
                     }
                     );

    server_->route("/configure/<arg>/<arg>", QHttpServerRequest::Method::Get,
                     [&controller](int frequency, int debug, const QHttpServerRequest &request) {
                         controller.configure(frequency, static_cast<bool>(debug));
                         return QHttpServerResponse(QString::fromStdString(controller.readResponse()));
                     }
                     );

    server_->route("/messages/<arg>", QHttpServerRequest::Method::Get,
                     [&controller](int numberOfMessagesToReturn, const QHttpServerRequest &request) {
                         controller.start();
                         return QHttpServerResponse(QString::fromStdString(controller.readResponse()));
                     }
                     );

    server_->route("/device", QHttpServerRequest::Method::Put,
                     [&controller](const QHttpServerRequest &request) {
                         controller.start();
                         return QHttpServerResponse(QString::fromStdString(controller.readResponse()));
                     }
                     );

    const auto port = server_->listen(QHostAddress::Any);

    if (!port)
    {
        // error
        //return 0;
    }

    qDebug() << QCoreApplication::translate("HttpServer", "Running on http://127.0.0.1:%1/").arg(port);
}

Server::~Server() = default;
