#include "serial_talker.hpp"

#include <QDebug>
#include <QSerialPort>

SerialTalker::SerialTalker(std::string name)
    : portName_{std::move(name)}
    , readCallback_{[](auto const& msg){ qDebug() << msg; }}
{
    openPort();
}

SerialTalker::~SerialTalker()
{
    closePort();
}

auto SerialTalker::setOnReadCallback(ReadCallback&& callback) -> void
{
    readCallback_ = std::move(callback);
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
    stream_.setDevice(port);

    QObject::connect(port, &QIODevice::readyRead, port,
                     [this]()
                     {
                        while(!stream_.atEnd())
                        {
                            readCallback_(read());
                        }
                     });
    port->setPortName(QString::fromStdString(portName_));
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
        qDebug() << __func__ << ": " << QString::fromStdString(portName_);
        dev->close();
        stream_.setDevice(nullptr);
        dev->deleteLater();
    }
}

