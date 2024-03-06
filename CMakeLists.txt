cmake_minimum_required(VERSION 3.10)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(version 0.0.1)

project(numbers VERSION ${version} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes.
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(THIRD_PARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/third_party
)

include_directories(${SRC_INCLUDE_DIR}  ${THIRD_PARTY_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(third_party)
add_subdirectory(tests)
