#pragma once

#include <QObject>
#include <QTextStream>

#include <functional>
#include <string>

class QSerialPort;

class SerialTalker : public QObject
{
    Q_OBJECT

    using ReadCallback = std::function<void(const std::string&)>;
public:
    SerialTalker(QString name);
    ~SerialTalker();

    auto setOnReadCallback(const ReadCallback& callback) -> void;
    auto write(const std::string& msg) -> void;

private:
    auto read() -> std::string;

    auto openPort() -> void;
    auto closePort() -> void;
private:
    ReadCallback readCallback_;
    QTextStream stream_;
    QString portName_;
};
