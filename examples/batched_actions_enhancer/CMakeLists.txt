option(
    UROBOROS_BUILD_EXAMPLE_BATCHED_ACTIONS_ENHANCER
    "Build the \"batched_actions_enhancer\" example"
    ON
)

if(UROBOROS_BUILD_EXAMPLE_BATCHED_ACTIONS_ENHANCER)
    add_executable(
        batched_actions_enhancer
        main.cpp
        ../common/batched_actions_enhancer.hpp
    )
    target_link_libraries(batched_actions_enhancer PRIVATE uroboros)
    set_target_properties(
        batched_actions_enhancer
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
