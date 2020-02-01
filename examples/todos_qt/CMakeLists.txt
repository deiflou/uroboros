string(
    CONCAT
    UROBOROS_BUILD_EXAMPLE_TODOS_QT_COMMENT
    "Build the \"todos_qt\" example\n"
    "    - Required dependencies:\n"
    "       - Qt5 Widgets\n"
    "         url: https://www.qt.io/ (not included)"
)

option(
    UROBOROS_BUILD_EXAMPLE_TODOS_QT
    ${UROBOROS_BUILD_EXAMPLE_TODOS_QT_COMMENT}
    ON
)

if(UROBOROS_BUILD_EXAMPLE_TODOS_QT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_executable(
        todos_qt
        ../todos/actions.hpp
        main.cpp
        ../todos/reducer.cpp
        ../todos/state.hpp
        ../todos/types.hpp
        window.hpp
        window.cpp
    )
    target_link_libraries(todos_qt PRIVATE uroboros Qt5::Widgets)
    set_target_properties(
        todos_qt
        PROPERTIES
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
