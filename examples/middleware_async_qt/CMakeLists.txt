string(
    CONCAT
    UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_QT_COMMENT
    "Build the \"middleware_async_qt\" example.\n"
    "    - Required dependencies:\n"
    "       - stlab's concurrency library\n"
    "         url: https://github.com/stlab/libraries (not included)\n"
    "       - Qt5 Widgets\n"
    "         url: https://www.qt.io/ (not included)"
)

option(
    UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_QT
    ${UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_QT_COMMENT}
    ON
)

if(UROBOROS_BUILD_EXAMPLE_MIDDLEWARE_ASYNC_QT)
    find_package(stlab REQUIRED)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    
    add_executable(
        middleware_async_qt
        main.cpp
        window.hpp
        window.cpp
        reducer.cpp
        async_middleware.cpp
        types.hpp
    )
    target_link_libraries(
        middleware_async_qt
        PRIVATE 
        uroboros
        stlab::stlab
        Qt5::Widgets
    )
    set_target_properties(
        middleware_async_qt
        PROPERTIES
        CMAKE_AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
