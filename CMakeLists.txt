cmake_minimum_required(VERSION 3.15)
project(Project2019)

set(CMAKE_CXX_STANDARD 14)
link_libraries(cunit)

include_directories(.)
add_executable(Project2019
        radix.cpp
        radix.h
        inputFunctions.cpp
        inputFunctions.h
        main.cpp stack.cpp stack.h sort.cpp sort.h array.cpp array.h list.cpp list.h ResultsArray.cpp ResultsArray.h)

add_executable(test_radix
        radix.cpp
        radix.h
        inputFunctions.cpp
        inputFunctions.h
        test_radix.cpp stack.cpp stack.h sort.cpp sort.h)
