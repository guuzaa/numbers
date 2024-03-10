cmake_minimum_required(VERSION 3.10)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} PROJECT_IS_TOP_LEVEL)

if (PROJECT_IS_TOP_LEVEL)
        message(STATUS "Project is top level")
endif()

option(NUMBERS_TEST "Build and perform numbers tests" ${PROJECT_IS_TOP_LEVEL})

set(version 0.0.2)

project(numbers VERSION ${version} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes.
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

include_directories(${SRC_INCLUDE_DIR})

add_subdirectory(src)

if (NUMBERS_TEST) 
        message(STATUS "Building and running tests")
        set(THIRD_PARTY_INCLUDE_DIR
                ${PROJECT_SOURCE_DIR}/third_party
        )
        include_directories(${THIRD_PARTY_INCLUDE_DIR})

        add_subdirectory(examples)
        add_subdirectory(third_party)
        add_subdirectory(tests)
endif()
