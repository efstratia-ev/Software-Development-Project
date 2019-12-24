cmake_minimum_required(VERSION 3.13)
project(Project2019)
#add_compile_options(-O3)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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
        join_preds.h join_preds.cpp
        SQL.h SQL.cpp Priority_Queue.cpp Priority_Queue.h
        queue.h queue.cpp 
        jobscheduler.h jobscheduler.cpp)

add_executable(test_radix
        radix.cpp
        radix.h
        test_radix.cpp stack.cpp stack.h sort.cpp sort.h
        relation.h relation.cpp
        list.h list.cpp
        array.h array.cpp)

add_executable(test_relations
        test_relations.cpp
        relation.h
        relations.h
)

add_executable(test_join
        test_join.cpp
        Priority_Queue.cpp Priority_Queue.h
        join_preds.cpp join_preds.h
        SQLInfo.h SQLInfo.cpp
        radix.cpp radix.h
        stack.cpp stack.h
        sort.cpp sort.h
        array.cpp array.h
        list.cpp list.h
        JoinArray.h JoinArray.cpp
        relations.cpp relation.cpp
        results_list.cpp results_list.h
        Priority_Queue.cpp Priority_Queue.h
        SQL.h SQL.cpp)

add_executable(test_queue
        test_queue.cpp 
        queue.h queue.cpp
        job.h
)

add_executable(test_jobscheduler
        test_jobscheduler.cpp 
        jobscheduler.h jobscheduler.cpp 
        queue.h queue.cpp
        job.h 
)

target_link_libraries(Project2019 Threads::Threads)
target_link_libraries(test_jobscheduler Threads::Threads)