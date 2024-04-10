#include "serial_talker.hpp"

#include <QDebug>
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
            // invalid baudrate
            baudrate = 115000;
        }
    }

    int baudrate;
};

}

SerialTalker::SerialTalker(QString name)
    : portName_{std::move(name)}
    , readCallback_{[](auto const& msg){ qDebug() << "no read callback set"; }}
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
        qDebug() << "Error: " << port->errorString();
    }
    else
    {
        qDebug() << "Sucess!";
    }
}

auto SerialTalker::closePort() -> void
{
    if(auto dev = stream_.device(); dev)
    {
        qDebug() << __func__ << ": " << portName_;
        dev->close();
        stream_.setDevice(nullptr);
        dev->deleteLater();
    }
}

