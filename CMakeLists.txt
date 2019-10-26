cmake_minimum_required(VERSION 3.15)
project(Project2019)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_executable(Project2019
        hashtable.cpp
        hashtable.h
        inpoutFunctions.cpp
        inpoutFunctionsLibrary.h
        list.cpp
        list.h
        main.cpp stack.cpp stack.h)
