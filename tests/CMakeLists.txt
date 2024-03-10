enable_testing()

include(GoogleTest)

set(SRC_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/tests/include)
include_directories(${SRC_TEST_INCLUDE_DIR})

add_subdirectory(utils)
add_subdirectory(integer)
add_subdirectory(uinteger)

add_custom_target(tests)
add_dependencies(tests test_utils integer_test uinteger_test)

add_custom_target(run-tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${TEST_TARGETS} tests
    COMMENT "Build and Run All Tests..."
)

add_custom_target(check-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS ${TEST_TARGETS} 
    COMMENT "Running All Tests..."
)