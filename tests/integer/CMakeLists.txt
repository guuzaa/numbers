file(GLOB_RECURSE test_files "${CMAKE_CURRENT_SOURCE_DIR}/*test.cc")

set(PROJECT_NAME integer_test)

add_executable(
        ${PROJECT_NAME}
        EXCLUDE_FROM_ALL
        ${test_files}
)

target_link_libraries(${PROJECT_NAME} PRIVATE numbers_obj test_utils gtest gmock_main)

gtest_discover_tests(${PROJECT_NAME}
        EXTRA_ARGS
        --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${PROJECT_NAME}.xml
        --gtest_catch_exceptions=0
        DISCOVERY_TIMEOUT 120
        PROPERTIES
        TIMEOUT 120
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

list(APPEND TEST_TARGETS ${PROJECT_NAME} PARENT_SCOPE)

# Add the test targets
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
