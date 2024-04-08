cmake_minimum_required(VERSION 3.16)

set(PROJECT clone_device-controller)
project(${PROJECT} LANGUAGES CXX)

#BUILD FLAGS ####################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

#PREPARING HEADER FILES ################################
file(GLOB_RECURSE EXTERNAL_HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES
    include/*.cpp src/*.cpp)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core SerialPort)

#CREATING EXEC AND LINK IT WITH LIBS AND MANUALLY CREATED SOURCES ###############
add_library(${PROJECT} ${SOURCES} ${EXTERNAL_HEADERS})
target_include_directories(${PROJECT}
    PRIVATE src
    PUBLIC include)
target_link_libraries(${PROJECT}
    PRIVATE
    clone_serial-talker
)

add_compile_options("/std:c++latest")
target_compile_definitions(${PROJECT} PRIVATE clone_device-controller_LIBRARY)
