#pragma once

#include <QObject>
#include <QTextStream>

#include <functional>
#include <string>

class QSerialPort;

class SerialTalker : public QObject
{
    Q_OBJECT

    using ReadCallback = std::function<void(std::string&&)>;
public:
    SerialTalker(std::string name);
    ~SerialTalker();

    auto setOnReadCallback(ReadCallback&& callback) -> void;
    auto write(const std::string& msg) -> void;

private:
    auto read() -> std::string;

    auto openPort() -> void;
    auto closePort() -> void;
private:
    ReadCallback readCallback_;
    QTextStream stream_;
    std::string portName_;
};
