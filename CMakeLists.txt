cmake_minimum_required(VERSION 3.21)
project(WindowsApplication)
set(CMAKE_CXX_STANDARD 14)
add_executable(WindowsApplication Image.cpp Image.h UsefulFunctions.cpp UsefulFunctions.h main.cpp)
