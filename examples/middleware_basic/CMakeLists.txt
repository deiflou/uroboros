option(
    UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_BASIC
    "Build the \"middleware_basic\" example"
    ON
)

if(UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_BASIC)
    add_executable(middleware_basic main.cpp)
    target_link_libraries(middleware_basic PRIVATE uroboros)
    set_target_properties(
        middleware_basic
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
