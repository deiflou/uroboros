string(
    CONCAT
    UROBOROS_BUILD_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_COMMENT
    "Build the \"function_type_specialization\" example.\n"
    "    - Optional dependencies:\n"
    "       - Naios' function2 library\n"
    "         url: https://github.com/Naios/function2 (included as a submodule)\n"
    "       - skarupke's std_function libary\n"
    "         url: https://github.com/skarupke/std_function (included as a submodule)\n"
    "       - pmed's fixed_size_function library\n"
    "         url: https://github.com/pmed/fixed_size_function (included as a submodule)"
)

option(
    UROBOROS_BUILD_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION
    ${UROBOROS_BUILD_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_COMMENT}
    ON
)

if(UROBOROS_BUILD_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION)
    set(
        UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES
        uroboros
    )
    
    find_package(naios_function2)
    if(NAIOS_FUNCTION2_FOUND)
        set(
            UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES
            ${UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES}
            naios_function2
        )
    endif()
    
    find_package(pmed_fixed_size_function)
    if(PMED_FIXED_SIZE_FUNCTION_FOUND)
        set(
            UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES
            ${UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES}
            pmed_fixed_size_function
        )
    endif()
    
    find_package(skarupke_std_function)
    if(SKARUPKE_STD_FUNCTION_FOUND)
        set(
            UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES
            ${UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES}
            skarupke_std_function
        )
    endif()

    add_executable(function_type_specialization main.cpp)
    target_link_libraries(
        function_type_specialization
        PRIVATE
        ${UROBOROS_EXAMPLE_FUNCTION_TYPE_SPECIALIZATION_LIBRARIES}
    )
    set_target_properties(
        function_type_specialization
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${UROBOROS_EXAMPLES_OUTPUT_DIRECTORY}
    )
endif()
