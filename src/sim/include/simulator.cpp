#include "simulator.hpp"

#include "device.h"
#include <loguru.hpp>

#include <QProcessEnvironment>
#include <thread>
#include <signal.h>

Simulator::Simulator()
{
    LOG_F(INFO, "Fork for simulation..");
    child_pid_ = fork();
    if(child_pid_ == 0)
    {
        const auto sysEnv = QProcessEnvironment::systemEnvironment();
        const auto arg1 = QString("pty,raw,link=%1,echo=0\0").arg(sysEnv.value("SIM_SERIAL_PORT_1", "/tmp/sim1")).toStdString();
        const auto arg2 = QString("pty,raw,link=%1,echo=0\0").arg(sysEnv.value("SIM_SERIAL_PORT_2", "/tmp/sim2")).toStdString();

        const char* argv[5];
        argv[0] = "-d";
        argv[1] = "-d";
        argv[2] = arg1.c_str();
        argv[3] = arg2.c_str();
        argv[4] = NULL;

        LOG_F(INFO, "Starting socat from child, args: %s %s %s %s", argv[0], argv[1], argv[2], argv[3]);
        execvp("socat", const_cast<char**>(argv));
        auto errorCode = errno;

        LOG_F(ERROR, "execvp failed, error code: %d, calling exit..", errorCode);
        exit(errorCode);
    }
    else
    {
        LOG_F(INFO, ".. fork pid: %d", child_pid_);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    device_ = std::make_unique<Device>();
}

Simulator::~Simulator()
{
    if(child_pid_ > 0)
    {
        kill(child_pid_, SIGTERM);  // what if child failed to execute ?
    }
};
