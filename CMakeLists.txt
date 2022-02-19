#cmake version
cmake_minimum_required(VERSION 3.18.4)
#cmake project
project(GyyBot VERSION 0.01)
#C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#include dir
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

#make executable
add_executable(${PROJECT_NAME} ${SOURCES})

