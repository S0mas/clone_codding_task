cmake_minimum_required(VERSION 3.16)

set(PROJECT clone_common)
project(${PROJECT} LANGUAGES CXX)

#BUILD FLAGS ####################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#PREPARING HEADER FILES ################################
file(GLOB_RECURSE EXTERNAL_HEADERS include/*.hpp)

#CREATING LIB AND LINK IT WITH LIBS AND MANUALLY CREATED SOURCES ###############
add_library(${PROJECT} ${EXTERNAL_HEADERS}
    include/message_id.hpp)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT}
    PUBLIC include)

add_compile_options("/std:c++latest")
target_compile_definitions(${PROJECT} PRIVATE clone_common_LIBRARY)
