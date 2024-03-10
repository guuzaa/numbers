cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE EXAMPLE_SOURCES "${PROJECT_SOURCE_DIR}/examples/*.cc")

add_custom_target(example)

foreach (example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_filename ${example_source} NAME)
    string(REPLACE ".cc" "" example_name ${example_filename})
    add_executable(${example_name} EXCLUDE_FROM_ALL ${example_source})

    add_custom_command(
        TARGET example
        COMMENT "Running example ${example_name}..."
        COMMAND $<TARGET_FILE:${example_name}>
        USES_TERMINAL
    )

    target_link_libraries(${example_name} PRIVATE numbers_obj)

    set_target_properties(${example_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
            COMMAND ${example_name}
    )

    add_custom_target(example-${example_name}
        COMMENT "Running example ${example_name}..."
        COMMAND ${example_name}
    )
endforeach ()
