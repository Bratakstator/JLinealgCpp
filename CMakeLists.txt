cmake_minimum_required(VERSION 4.1)
project(JLinealgCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(JLinealgCpp src/main.cpp
        src/objects/Matrix.h
        src/objects/Vector.h
        src/logging/Logger.h)
