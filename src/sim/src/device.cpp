#include "device.h"

#include <measurement.hpp>
#include <loguru.hpp>
#include <parser.hpp>
#include <serial_talker.hpp>

#include <QProcessEnvironment>
#include <QTimer>

#include <random>

Q_DECLARE_METATYPE(Configuration)

namespace
{

float getRandomFloat()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 0xFFFF); // range [0, 1)
    return dis(e);
}

Measurement getRandomMeasurement()
{
    Measurement meas;
    meas.pressure_ = getRandomFloat();
    meas.temperature_ = getRandomFloat();
    meas.velocity_ = getRandomFloat();
    return meas;
}

struct DeviceSerialConfig
{
    DeviceSerialConfig()
    {
        port = QProcessEnvironment::systemEnvironment().value("SIM_SERIAL_PORT_1", "/tmp/sim1");
    }

    QString port;
};

}

Device::Device(QObject* parent)
    : QObject(parent)
    , messageSenderTimer_{nullptr}
{
    setup();
}

Device::~Device() = default;

auto Device::startTransmission() const -> void
{
    LOG_F(INFO, "Device sim received start transmission request");
    messageSenderTimer_->start();
    responseSuccess(currentMsg_);
}

auto Device::stopTransmission() const -> void
{
    LOG_F(INFO, "Device sim received stop transmission request");
    messageSenderTimer_->stop();
    responseSuccess(currentMsg_);
}

auto Device::setConfiguration(const Configuration& newConfig) -> void
{
    LOG_F(INFO, "Device sim received start configuration request, frequency: %d, debug: %d", newConfig.frequency_, newConfig.debug_);
    configuration_ = newConfig;
    setTimer(configuration_.frequency_);
    responseSuccess(currentMsg_);
}

auto Device::setup() -> void
{
    QObject::connect(&messageProcessor_, &MessageProcessor::startTransmission, this, &Device::startTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::stopTransmission, this, &Device::stopTransmission);
    QObject::connect(&messageProcessor_, &MessageProcessor::setConfiguration, this, &Device::setConfiguration);
    QObject::connect(&messageProcessor_, &MessageProcessor::invalidMessage, this, &Device::reportError);
    QObject::connect(&messageProcessor_, &MessageProcessor::invalidMessage, this, &Device::responseFailure);

    serialTalker_ = std::make_unique<SerialTalker>(DeviceSerialConfig().port);
    messageSenderTimer_ = new QTimer(this);
    messageSenderTimer_->setInterval(1000);
    QObject::connect(messageSenderTimer_, &QTimer::timeout, this, &Device::sendMeasurement);
    serialTalker_->setOnReadCallback([this](auto const& msg)
                                     {
                                         LOG_F(INFO, "Device sim received start msg, msg: %s", msg.c_str());
                                         currentMsg_ = msg;
                                         messageProcessor_.processMessage(msg);
                                     });
}

auto Device::sendMeasurement() const -> void
{
    serialTalker_->write(Parser::toMsg(getRandomMeasurement()));
}

auto Device::setTimer(const double frequency) const -> void
{
    if(frequency == 0)
    {
        LOG_F(WARNING, "Frequency set to 0, setting the measurement interval to %d ms", std::numeric_limits<int>::max());
        messageSenderTimer_->setInterval(std::numeric_limits<int>::max());
    }
    else
    {
        const auto sec = 1000.0; //ms
        messageSenderTimer_->setInterval(sec/frequency);
    }
}

auto Device::reportError(const std::string& msg, const std::string& errorMsg) const -> void
{
    LOG_F(ERROR, "Device sim reports error on msg: %s, error: %s", msg.c_str(), errorMsg.c_str());
}

auto Device::responseSuccess(const std::string& msg) const -> void
{
    response(msg + ",ok");
}

auto Device::responseFailure(const std::string& msg, const std::string& errorMsg) const -> void
{
    response(msg + "," + errorMsg);
}

auto Device::response(const std::string& msg) const -> void
{
    LOG_F(INFO, "Device sim sends response, response: %s", msg.c_str());
    serialTalker_->write(msg);
}
