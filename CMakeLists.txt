cmake_minimum_required(VERSION 3.13)
project(Project2019)

set(CMAKE_CXX_STANDARD 14)
link_libraries(cunit)

include_directories(.)
add_executable(Project2019
        SQLInfo.h SQLInfo.cpp
        radix.cpp radix.h
        main.cpp
        stack.cpp stack.h
        sort.cpp sort.h
        array.cpp array.h
        list.cpp list.h
        JoinArray.h JoinArray.cpp
        relations.cpp relation.cpp
        results_list.cpp results_list.h
        SQL.h SQL.cpp)

add_executable(test_radix
        radix.cpp
        radix.h
        test_radix.cpp stack.cpp stack.h sort.cpp sort.h)

add_executable(test_relations
        test_relations.cpp
        relation.h
        relations.h
)

add_executable(test testi.cpp)