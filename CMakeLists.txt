cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(SDL3 CONFIG REQUIRED)

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld PRIVATE SDL3::SDL3)