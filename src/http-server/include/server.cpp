#include "server.hpp"

#include <QtHttpServer>
#include <QJsonDocument>

#include <optional>

struct config
{
    int frequency = 0;
    bool debug = false;
};

static std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);
    if (err.error || !json.isObject())
        return std::nullopt;
    return json.object();
}

std::optional<config> configFromJson(const QJsonObject &json)
{
    if(json.contains("frequency") && json.contains("debug"))
    {
        return config{json.value("frequency").toInt(), json.value("debug").toBool()};
    }
    else
    {
        return std::nullopt;
    }
}

QHttpServerResponse addConfig(const QHttpServerRequest& request, DeviceController& controller)
{
    const std::optional<QJsonObject> json = byteArrayToJsonObject(request.body());
    if(!json)
    {
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
    }

    const auto& config = configFromJson(*json);
    if(!config.has_value())
    {
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
    }

    controller.configure(config.value().frequency, config.value().debug);
    return QHttpServerResponse(QHttpServerResponder::StatusCode::Ok);
}

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

    server_->route("/configure",
                   [&controller](const QHttpServerRequest &request) {
                       return addConfig(request, controller);
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
