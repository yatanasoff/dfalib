cmake_minimum_required(VERSION 3.0)
project(testmod)

add_definitions("-fno-stack-protector")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

include_directories(../../include)

add_executable(testmod "main.cpp" "ProgressBar.hpp" )
target_link_libraries(testmod dfalib regex)

