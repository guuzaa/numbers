add_library(numbers_lib STATIC ${ALL_OBJECT_FILES})

find_package(Threads REQUIRED)

set(THIRDPARTY_LIBS
        Threads::Threads
)

target_link_libraries(numbers_lib ${THIRDPARTY_LIBS})

target_include_directories(numbers_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEIR}>
)
