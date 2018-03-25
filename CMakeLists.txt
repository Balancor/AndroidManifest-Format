cmake_minimum_required(VERSION 3.9)
project(AndroidResourceTool)

set(CMAKE_CXX_STANDARD 11)

add_executable(AndroidResourceTool main.cpp ResourceType.h ResourceType.cpp)