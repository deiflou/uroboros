option(
    UROBOROS_BUILD_EXAMPLE_COUNTER
    "Build the \"counter\" example"
    ON
)

if(UROBOROS_BUILD_EXAMPLE_COUNTER)
    add_executable(counter main.cpp)
    target_link_libraries(counter PRIVATE uroboros)
    set_target_properties(
        counter
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
