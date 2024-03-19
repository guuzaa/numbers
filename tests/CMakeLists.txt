enable_testing()

include(GoogleTest)

set(SRC_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/tests/include)
include_directories(${SRC_TEST_INCLUDE_DIR})

add_subdirectory(utils)

set(test_dir_list "${CMAKE_CURRENT_SOURCE_DIR}/integer" "${CMAKE_CURRENT_SOURCE_DIR}/uinteger")
foreach(test_dir ${test_dir_list}) 
    message(STATUS "test_dir: ${test_dir}")

    file(GLOB_RECURSE test_files "${test_dir}/*.test.cc")
    get_filename_component(dir_name ${test_dir} NAME)
    message(STATUS "dir_name: ${dir_name}")

    set(PROJECT_NAME ${dir_name}_test)

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

    add_custom_target(test-${dir_name}
        COMMENT "Running test ${PROJECT_NAME}..."
        COMMAND ${PROJECT_NAME}
    )

    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
    )

    list(APPEND TEST_TARGETS ${PROJECT_NAME})

    # Add the test targets
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endforeach()

add_custom_target(tests)
add_dependencies(tests test_utils ${TEST_TARGETS})

add_custom_target(run-tests
    COMMAND ${CMAKE_CTEST_COMMAND} -C Debug
    DEPENDS tests 
    COMMENT "Build and Run All Tests..."
)

add_custom_target(check-tests
    COMMAND ${CMAKE_CTEST_COMMAND} -C Debug --verbose
    DEPENDS ${TEST_TARGETS} 
    COMMENT "Running All Tests..."
)