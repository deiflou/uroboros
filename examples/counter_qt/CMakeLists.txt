string(
    CONCAT
    UROBOROS_BUILD_EXAMPLE_COUNTER_QT_COMMENT
    "Build the \"counter_qt\" example.\n"
    "    - Required dependencies:\n"
    "       - Qt5 Widgets\n"
    "         url: https://www.qt.io/ (not included)"
)

option(
    UROBOROS_BUILD_EXAMPLE_COUNTER_QT
    ${UROBOROS_BUILD_EXAMPLE_COUNTER_QT_COMMENT}
    ON
)

if(UROBOROS_BUILD_EXAMPLE_COUNTER_QT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    
    add_executable(counter_qt ${UROBOROS_WIN32_EXECUTABLE} main.cpp)
    target_link_libraries(counter_qt PRIVATE uroboros Qt5::Widgets)
    set_target_properties(
        counter_qt
        PROPERTIES
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
