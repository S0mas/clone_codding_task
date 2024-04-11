#include "serial_talker.hpp"

#include <loguru.hpp>

#include <QSerialPort>
#include <QProcessEnvironment>

namespace
{

struct SerialConfig
{
    SerialConfig()
    {
        bool okBaudRate;
        baudrate = QProcessEnvironment::systemEnvironment().value("SERIAL_BAUDRATE", "115000").toInt(&okBaudRate);
        if(!okBaudRate)
        {
            baudrate = 115000;
            LOG_F(ERROR, "SERIAL_BAUDRATE value is invalid, default will be used, default: %d", baudrate);
        }
    }

    int baudrate;
};

}

SerialTalker::SerialTalker(QString name)
    : portName_{std::move(name)}
    , readCallback_{[this](auto const& msg){ LOG_F(ERROR, "No read callback set for serial port, name: %s", portName_.toStdString().c_str()); }}
{
    openPort();
}

SerialTalker::~SerialTalker()
{
    closePort();
}

auto SerialTalker::setOnReadCallback(const ReadCallback& callback) -> void
{
    readCallback_ = callback;
}

auto SerialTalker::read() -> std::string
{
    QString line;
    line = stream_.readLine();
    return line.toStdString();
}

auto SerialTalker::write(const std::string& msg) -> void
{
    stream_ << QString::fromStdString(msg) << "\n";
    stream_.flush();
}

auto SerialTalker::openPort() -> void
{
    auto port = new QSerialPort(this);
    port->setBaudRate(SerialConfig().baudrate);
    stream_.setDevice(port);

    QObject::connect(port, &QIODevice::readyRead, port,
                     [this]()
                     {
                        while(!stream_.atEnd())
                        {
                            readCallback_(read());
                        }
                     });
    port->setPortName(portName_);
    const auto result = port->open(QIODeviceBase::ReadWrite | QIODeviceBase::ExistingOnly);
    if(!result)
    {
        LOG_F(ERROR, "Failed to open serial port, name: %s, reason: %s", portName_.toStdString().c_str(), port->errorString().toStdString().c_str());
    }
    else
    {
        LOG_F(INFO, "Opened serial port, name: %s", portName_.toStdString().c_str());
    }
}

auto SerialTalker::closePort() -> void
{
    if(auto dev = stream_.device(); dev)
    {
        LOG_F(INFO, "Closed serial port, name: %s", portName_.toStdString().c_str());
        dev->close();
        stream_.setDevice(nullptr);
        dev->deleteLater();
    }
}

