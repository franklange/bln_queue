cmake_minimum_required(VERSION 3.10)
project(bln_queue)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS fiber)

option(BLN_QUEUE_TESTS "Enable BLN Queue Tests" OFF)
option(BLN_QUEUE_EXAMPLES "Enable BLN Examples" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
    include/bln_queue/types.hpp
    include/bln_queue/bi_queue.hpp
    include/bln_queue/data_queue.hpp
    include/bln_queue/msg_queue.hpp
    include/bln_queue/spinlock.hpp

    src/spinlock.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::fiber
    Threads::Threads
)

if (BLN_QUEUE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (BLN_QUEUE_EXAMPLES)
    add_subdirectory(example)
endif()
