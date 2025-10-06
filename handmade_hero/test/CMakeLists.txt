cmake_minimum_required(VERSION 3.10)

project(test)

add_executable(test main.cpp)

install(TARGETS test RUNTIME DESTINATION bin)
