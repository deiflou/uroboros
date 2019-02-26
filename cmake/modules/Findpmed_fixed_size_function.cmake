if(TARGET pmed_fixed_size_function)
    return()
endif()

find_file(
    PMED_FIXED_SIZE_FUNCTION_HEADER
    "/third_party/function_libraries/pmed/fixed_size_function/fixed_size_function.hpp"
    ${CMAKE_SOURCE_DIR}
)
if(NOT PMED_FIXED_SIZE_FUNCTION_HEADER STREQUAL "PMED_FIXED_SIZE_FUNCTION_HEADER-NOTFOUND")
    set(
        PMED_FIXED_SIZE_FUNCTION_INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/function_libraries/pmed"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    pmed_fixed_size_function
    DEFAULT_MSG
    PMED_FIXED_SIZE_FUNCTION_HEADER
    PMED_FIXED_SIZE_FUNCTION_INCLUDE_DIRS
)

if(PMED_FIXED_SIZE_FUNCTION_FOUND)
    add_library(pmed_fixed_size_function INTERFACE)
    target_include_directories(
        pmed_fixed_size_function
        INTERFACE
        ${PMED_FIXED_SIZE_FUNCTION_INCLUDE_DIRS}
    )
    target_compile_definitions(
        pmed_fixed_size_function
        INTERFACE
        PMED_FIXED_SIZE_FUNCTION_FOUND
    )
    add_library(
        pmed_fixed_size_function::pmed_fixed_size_function
        ALIAS
        pmed_fixed_size_function
    )
endif()
