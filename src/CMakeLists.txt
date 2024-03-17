add_subdirectory(numbers)

add_library(${PROJECT_NAME} STATIC ${ALL_OBJECT_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEIR}>
)
