cmake_minimum_required(VERSION 3.16)

set(PROJECT clone_gui)
project(${PROJECT} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(${PROJECT}
    main.cpp
)

qt_add_qml_module(${PROJECT}
    URI mtg
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES src/device_if.hpp src/device_if.cpp
)

target_link_libraries(${PROJECT}
    PRIVATE Qt6::Quick
    clone_sim
    clone_device-controller
    clone_http-server
)
