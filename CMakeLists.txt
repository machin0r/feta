cmake_minimum_required(VERSION 3.10)
project(feta)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(feta
    src/main.cpp
    src/STLReader.cpp
    src/Geometry.cpp
    src/Slicer.cpp
)