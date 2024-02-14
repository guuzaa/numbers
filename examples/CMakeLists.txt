cmake_minimum_required(VERSION 3.10)

project(NumbersExample LANGUAGES CXX)

add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL main.cc)

add_custom_target(example
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running Example..."
)