project(bln_queue_test)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PROJECT_NAME} "")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

target_sources(${PROJECT_NAME} PRIVATE
    bi_queue_TEST.cpp
    data_queue_TEST.cpp
    msg_queue_TEST.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    bln_queue
    GTest::Main
)

gtest_discover_tests(${PROJECT_NAME})
