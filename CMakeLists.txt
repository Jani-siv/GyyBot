#cmake version
cmake_minimum_required(VERSION 3.18.4)
#cmake project
project(GyyBot VERSION 0.01)
#C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#add subdirectories
add_subdirectory(src/filehandle)
#make executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC filehandle)

target_include_directories(${PROJECT_NAME} PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/filehandle")

