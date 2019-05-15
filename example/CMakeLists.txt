# Generate exmaples directory
set(EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Include header
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Custom macro to add a example
macro(add_example TARGET_NAME)
    # Set the right path
    set(EXAMPLE_DIR "${EXAMPLES_DIR}/${TARGET_NAME}")

    # Create right source path
    set(LIST_SOURCE_FILES "${ARGN}")
    set(NEW_LIST "")

    # Append prefix path
    foreach(SOURCE_FILE IN LISTS LIST_SOURCE_FILES)
        list(APPEND NEW_LIST "${EXAMPLE_DIR}/${SOURCE_FILE}")
    endforeach()
    set(LIST_SOURCE_FILES ${NEW_LIST})

    # Create the executable
    add_executable(${TARGET_NAME} ${LIST_SOURCE_FILES})
    target_link_libraries(${TARGET_NAME} dsps)
endmacro()

add_example(cascaded_filters src/main.cc)
