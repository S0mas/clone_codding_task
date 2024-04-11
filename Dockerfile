FROM ubuntu:23.04
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y socat
RUN apt-get install -y libx11-xcb-dev
RUN apt-get install -y libxkbcommon-dev
RUN apt-get install -y libxkbcommon-x11-dev
RUN apt-get install -y build-essential libgl1-mesa-dev
RUN apt-get install -y libxcb-icccm4
RUN apt-get install -y libxcb-image0-dev
RUN apt-get install -y libxcb-keysyms1-dev
RUN apt-get install -y libxcb-render-util0-dev
RUN apt-get install -y libxcb-shape0-dev
RUN apt-get install -y libxcb-cursor-dev
RUN apt-get install -y libmysqlclient-dev
RUN apt-get install -y git
RUN apt-get install -y cmake
RUN apt-get install -y ninja-build
RUN apt-get install -y libdbus-1-3
RUN apt-get install -y qt6-base-dev
RUN apt-get install -y qt6-base-dev-tools
RUN apt-get install -y qt6-declarative-dev
RUN apt-get install -y qt6-serialport-dev
RUN apt-get install -y qt6-serialport-dev
RUN apt-get install -y qt6-httpserver-dev
RUN apt-get install -y qt6-websockets-dev
RUN apt-get -y install qml6-module-qtquick
RUN apt-get -y install qml6-module-qtquick-window
RUN apt-get -y install qml6-module-qtquick-controls
RUN apt-get -y install qml6-module-qtquick-templates
RUN apt-get -y install qml6-module-qtquick-layouts
RUN apt-get -y install qml6-module-qtqml-workerscript

RUN git clone https://github.com/S0mas/clone_codding_task.git

RUN /usr/lib/qt6/bin/qt-cmake -G Ninja -S clone_codding_task/src -B clone_codding_task/build
RUN ninja -C clone_codding_task/build
