cmake_minimum_required(VERSION 3.16.0)

set(PROJECT src)
project(${PROJECT} LANGUAGES CXX)

#BUILD FLAGS ####################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(app)
add_subdirectory(serial-talker)
add_subdirectory(device-controller)
add_subdirectory(http-server)
add_subdirectory(database)
add_subdirectory(sim)
add_subdirectory(parser)
add_subdirectory(loguru)
