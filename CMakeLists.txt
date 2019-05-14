cmake_minimum_required(VERSION 3.13)
project(chat)

set(CMAKE_CXX_STANDARD 14)

add_executable(chat main.cpp Task.hpp chat.cpp chat.hpp)
