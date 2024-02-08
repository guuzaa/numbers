add_library(
        test_utils
        EXCLUDE_FROM_ALL
        OBJECT
        utils.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:test_utils>
        PARENT_SCOPE)