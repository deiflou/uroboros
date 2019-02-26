string(
    CONCAT
    UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_COMMENT
    "Build the \"middleware_async\" example.\n"
    "    - Required dependencies:\n"
    "       - stlab's concurrency library\n"
    "         url: https://github.com/stlab/libraries (not included)"
)

option(
    UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC
    ${UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_COMMENT}
    ON
)

if(UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC)
    find_package(stlab REQUIRED)
    
    add_executable(middleware_async main.cpp)
    target_link_libraries(
        middleware_async
        PRIVATE 
        uroboros
        stlab::stlab
    )
    set_target_properties(
        middleware_async
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
