cmake_minimum_required(VERSION 3.10)

set(version 0.0.3)

project(numbers VERSION ${version} LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)

string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} PROJECT_IS_IN_ROOT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ${PROJECT_IS_IN_ROOT})

if(PROJECT_IS_IN_ROOT)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    message(STATUS "Project ${PROJECT_NAME} is in root")
endif()

# Check if the compiler supports C++17
if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "Error: ${PROJECT_NAME} is a library for C++17 and later versions.")
endif()

option(NUMBERS_TEST "Build and perform ${PROJECT_NAME} tests" ${PROJECT_IS_IN_ROOT})
option(NUMBERS_EXAMPLE "Build and perform ${PROJECT_NAME} examples" ${PROJECT_IS_IN_ROOT})

# Includes.
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

include_directories(${SRC_INCLUDE_DIR})

add_subdirectory(src)

if(NUMBERS_EXAMPLE)
    message(STATUS "Building examples")
    add_subdirectory(examples)
endif()

if(NUMBERS_TEST)
    message(STATUS "Building tests")
    set(THIRD_PARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/third_party
    )
    include_directories(${THIRD_PARTY_INCLUDE_DIR})

    add_subdirectory(third_party)
    add_subdirectory(tests)

    set(BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
    set(BUSTUB_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin" "/usr/local/opt/llvm@14/bin"
        "/opt/homebrew/opt/llvm@14/bin/")

    find_program(CLANG_FORMAT_BIN
        NAMES clang-format clang-format-14
        HINTS ${CLANG_SEARCH_PATH}
    )

    if("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
        message(WARNING "numbers/main couldn't find clang-format.")
    else()
        message(STATUS "numbers/main found clang-format at ${CLANG_FORMAT_BIN}")
    endif()

    string(CONCAT FORMAT_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src,"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples,"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests,"
    )

    # Runs clang format and updates files in place.
    add_custom_target(format ${BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${FORMAT_DIRS}
        --fix
        --quiet
    )

    # Runs clang format and exits with a non-zero exit code if any files need to be reformatted
    add_custom_target(check-format ${BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${FORMAT_DIRS}
        --quiet
    )

    # Provide only the minimum source files needed by downstream users
    set(package_files src/ CMakeLists.txt LICENSE.txt)
    set(packaged_zip ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip)
    add_custom_command(
        OUTPUT ${packaged_zip}
        COMMAND ${CMAKE_COMMAND} -E tar c ${packaged_zip} --format=zip -- ${package_files}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "The source files have been packaged into ${packaged_zip}"
        DEPENDS ${package_files}
    )
    add_custom_target(package DEPENDS ${packaged_zip})
endif()
