cmake_minimum_required(VERSION 3.13)    # CMake version check
project(practice1)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
    src/main.cpp 
    src/parce.cpp 
    src/scene.cpp)
include_directories(include/)
add_executable(practice1 ${SOURCE_FILES})
