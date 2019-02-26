option(UROBOROS_BUILD_DOCS "Build the documentation. Requires Python and Sphinx" OFF)

if(UROBOROS_BUILD_DOCS)
    set(SPHINX_BUILD_EXECUTABLE_NAME sphinx-build)

    find_program(
        SPHINX_BUILD_EXECUTABLE ${SPHINX_BUILD_EXECUTABLE_NAME}
        DOC "Path to the \"${SPHINX_BUILD_EXECUTABLE_NAME}\" executable"
    )

    if("${SPHINX_BUILD_EXECUTABLE}" STREQUAL "SPHINX_BUILD_EXECUTABLE-NOTFOUND")
        uroboros_info_message(
            "The documentation will not be built."
            "Reason: \"${SPHINX_BUILD_EXECUTABLE_NAME}\" executable not found."
        )
    else()
        set(UROBOROS_DOCS_OUTPUT_DIRECTORY "${UROBOROS_OUTPUT_DIRECTORY}/docs")

        add_custom_target(
            doc ALL COMMAND
            "${SPHINX_BUILD_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/docs"
            ${UROBOROS_DOCS_OUTPUT_DIRECTORY}
            "-d \"${CMAKE_CURRENT_BINARY_DIR}/.doctrees\""
        )

        install(
            DIRECTORY "${UROBOROS_DOCS_OUTPUT_DIRECTORY}/"
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
        )
    endif()
endif()
