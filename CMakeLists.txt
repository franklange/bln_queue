cmake_minimum_required(VERSION 3.10)
project(bln_queue)

option(BLN_QUEUE_TESTS "Enable BLN Queue Tests" OFF)
option(BLN_QUEUE_EXAMPLES "Enable BLN Examples" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()

add_library(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
    include/bln_queue/queue.h
    include/bln_queue/spinlock.h

    src/spinlock.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if (BLN_QUEUE_TESTS)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_compile_options(-Wall -Wextra -pedantic)
    endif()

    enable_testing()
    add_subdirectory(test)
endif()

if (BLN_QUEUE_EXAMPLES)
    add_subdirectory(example)
endif()
