add_subdirectory(numbers)

add_library(numbers STATIC ${ALL_OBJECT_FILES})

target_include_directories(numbers PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEIR}>
)
