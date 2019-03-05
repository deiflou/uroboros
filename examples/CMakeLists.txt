option(UROBOROS_BUILD_EXAMPLES "Build the examples" OFF)

if(UROBOROS_BUILD_EXAMPLES)
    set(
        UROBOROS_EXAMPLES_OUTPUT_DIRECTORY
        "${UROBOROS_OUTPUT_DIRECTORY}/examples"
    )

    add_subdirectory(batched_actions_enhancer)
    add_subdirectory(counter)
    add_subdirectory(counter_qt)
    add_subdirectory(function_type_specialization)
    add_subdirectory(middleware_async)
    add_subdirectory(middleware_async_qt)
    add_subdirectory(middleware_basic)
    add_subdirectory(todos)
    add_subdirectory(todos_qt)
endif()
