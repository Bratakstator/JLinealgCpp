cmake_minimum_required(VERSION 4.1)
project(JLinealgCpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(JLinealgCpp src/main.cpp
        src/objects_OLD/Matrix/Matrix_operators.cpp
        src/objects_OLD/Matrix/Matrix.cpp
        src/objects_OLD/Span/Span.cpp
        src/objects_OLD/Span/Span_operators.cpp
        src/objects_OLD/Vector/Vector.cpp
        src/objects_OLD/Vector/Vector_operators.cpp
        src/objects_OLD/Vector/ComponentProxy.cpp
        src/objects_OLD/Span/VectorProxy.cpp
        src/objects_OLD/Matrix/Matrix_operations.cpp
        src/Tools/VectorTools.h
        src/objects_OLD/Span/ComponentPProxy.cpp
        src/objects_OLD/Matrix/ComponentPPProxy.cpp
        src/objects_OLD/Matrix/Matrix_QR_methods.cpp
)
