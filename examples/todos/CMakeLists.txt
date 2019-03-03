option(
    UROBOROS_BUILD_EXAMPLE_TODOS
    "Build the \"todos\" example"
    ON
)

if(UROBOROS_BUILD_EXAMPLE_TODOS)
    add_executable(
        todos
        actions.hpp
        main.cpp
        reducer.cpp
        state.hpp
        types.hpp
    )
    target_link_libraries(todos PRIVATE uroboros)
    set_target_properties(
        todos
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
