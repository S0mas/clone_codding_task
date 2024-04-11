#include "simulator.hpp"

#include "device.h"

#include <QProcessEnvironment>
#include <thread>
#include <signal.h>

Simulator::Simulator()
{
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

        execvp("socat", const_cast<char**>(argv));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    device_ = std::make_unique<Device>();
}

Simulator::~Simulator()
{
    kill(child_pid_, SIGTERM);
};
