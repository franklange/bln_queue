project(bln_queue_test)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PROJECT_NAME} queue_TEST.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "tests"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    bln_queue
    GTest::Main
)

gtest_discover_tests(${PROJECT_NAME})
