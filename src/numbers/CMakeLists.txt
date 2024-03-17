file(GLOB obj_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

add_library(
    numbers_obj
    OBJECT
    ${obj_files}
)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:numbers_obj>
    PARENT_SCOPE)