#include "server.hpp"

#include <configuration.hpp>
#include <parser.hpp>
#include <loguru.hpp>

#include <QtHttpServer>
#include <QJsonDocument>
#include <QProcessEnvironment>

#include <chrono>
#include <optional>

namespace
{

struct ServerConfig
{
    ServerConfig()
    {
        bool okPort;
        port = QProcessEnvironment::systemEnvironment().value("SERVER_PORT", "7100").toInt(&okPort);
        if(!okPort)
        {
            LOG_F(WARNING, "SERVER_PORT value is invalid, using default port: %d", port);
            port = 7100;
        }

        address = QHostAddress(QProcessEnvironment::systemEnvironment().value("SERVER_ADDRESS", ""));
        if(address.isNull())
        {
            LOG_F(WARNING, "SERVER_ADDRESS value is empty or invalid, using default port: %d", port);
            address = QHostAddress::LocalHost;
        }
    }

    int port;
    QHostAddress address;
};

}

static std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);
    if(err.error || !json.isObject())
    {
        return std::nullopt;
    }
    return json.object();
}

std::optional<Configuration> configFromJson(const QJsonObject &json)
{
    if(json.contains("frequency") && json.contains("debug"))
    {
        std::string debug_token;
        if(json.value("debug").isBool())
        {
            debug_token = std::to_string(json.value("debug").toBool());
        }
        else if(json.value("debug").isString())
        {
            debug_token = json.value("debug").toString().toStdString();
        }
        else if(auto value = json.value("debug").toInt(-1); value == 1 || value == 0)
        {
            debug_token = std::to_string(value);
        }

        return Parser::parseConfigFromTokens(std::to_string(json.value("frequency").toInt()), debug_token);
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

    controller.configure(config.value());
    return QHttpServerResponse(QHttpServerResponder::StatusCode::Ok);
}

Server::Server(DeviceController& controller)
    : controller_{controller}
    , server_{std::make_unique<QHttpServer>()}
{
    server_->route("/start", QHttpServerRequest::Method::Get,
                   [&controller, this](const QHttpServerRequest &request) {
                       LOG_F(INFO, "Server recived start transmission request");
                       controller.start();
                       return sendResponse();
                   }
                   );

    server_->route("/stop", QHttpServerRequest::Method::Get,
                   [&controller, this](const QHttpServerRequest &request) {
                       LOG_F(INFO, "Server recived stop transmission request");
                       controller.stop();
                       return sendResponse();
                   }
                   );

    server_->route("/configure",
                   [&controller, this](const QHttpServerRequest &request) {
                       LOG_F(INFO, "Server recived configure request");
                       return addConfig(request, controller);
                   }
                   );

    server_->route("/messages/<arg>", QHttpServerRequest::Method::Get,
                   [&controller, this](int numberOfMessagesToReturn, const QHttpServerRequest &request) {
                       LOG_F(INFO, "Server recived messages request");
                       controller.start();
                       return sendResponse();
                   }
                   );

    server_->route("/device", QHttpServerRequest::Method::Put,
                   [&controller, this](const QHttpServerRequest &request) {
                       LOG_F(INFO, "Server recived device request");
                       controller.start();
                       return sendResponse();
                   }
                   );

    if(auto port = server_->listen(ServerConfig().address, ServerConfig().port); port != 0)
    {
        LOG_F(INFO, "HttpServer, running on http://%s:%d/", ServerConfig().address.toString().toStdString().c_str(), port);
    }
    else
    {
        LOG_F(ERROR, "Failed to start server on http://%s:%d/", ServerConfig().address.toString().toStdString().c_str(), port);
    }
}

Server::~Server() = default;

auto Server::setNewResponse(const std::string& msg) -> void
{
    response_ = msg;
}

auto Server::takeResponse() -> std::string
{
    auto msg = response_.value();
    response_.reset();
    return msg;
}

auto Server::waitForResponse(int ms) -> void
{
    auto current_time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    const auto end_time_in_ms = current_time_in_ms + ms;
    while(!response_.has_value() && current_time_in_ms <= end_time_in_ms)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        current_time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
}

auto Server::sendResponse() -> QHttpServerResponse
{
    waitForResponse();
    if(response_.has_value())
    {
        LOG_F(INFO, "Server recived response from the device, response: %s", response_.value().c_str());
        return QHttpServerResponse(QString::fromStdString(takeResponse()));
    }
    else
    {
        LOG_F(ERROR, "Timeout reached while server was waiting for the response from the device");
        return QHttpServerResponse(QHttpServerResponse::StatusCode::RequestTimeout);
    }
}
