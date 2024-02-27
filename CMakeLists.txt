cmake_minimum_required(VERSION 3.13)    # CMake version check
project(practice)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
    src/main.cpp 
    src/parce.cpp 
    src/scene.cpp 
    src/primitives.cpp 
    src/geometry.cpp
    src/color.cpp)
include_directories(include/)
add_executable(practice ${SOURCE_FILES})
