cmake_minimum_required(VERSION 3.0.0)
project(ludoGame VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB SOURCES
    *.h
    *.cpp
)

set(CMAKE_CXX_FLAGS -pthread)

add_executable(ludoGame main.cpp ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
