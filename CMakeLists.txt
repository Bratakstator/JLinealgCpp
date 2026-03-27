cmake_minimum_required(VERSION 4.1)
project(JLinealgCpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(JLinealgCpp src/main.cpp
        src/objects/Matrix/Matrix_operators.cpp
        src/objects/Matrix/Matrix.cpp
        src/objects/Span/Span.cpp
        src/objects/Span/Span_operators.cpp
        src/objects/Vector/Vector.cpp
        src/objects/Vector/Vector_operators.cpp
        src/objects/Vector/DoubleProxy.cpp
        src/objects/Span/VectorProxy.cpp
        src/objects/Matrix/Matrix_operations.cpp
        src/Tools/VectorTools.h
)
