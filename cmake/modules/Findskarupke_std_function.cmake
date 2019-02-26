if(TARGET skarupke_std_function)
    return()
endif()

find_file(
    SKARUPKE_STD_FUNCTION_HEADER
    "/third_party/function_libraries/skarupke/std_function/function.h"
    ${CMAKE_SOURCE_DIR}
)
if(NOT SKARUPKE_STD_FUNCTION_HEADER STREQUAL "SKARUPKE_STD_FUNCTION_HEADER-NOTFOUND")
    set(
        SKARUPKE_STD_FUNCTION_INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/function_libraries/skarupke"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    skarupke_std_function
    DEFAULT_MSG
    SKARUPKE_STD_FUNCTION_INCLUDE_DIRS
    SKARUPKE_STD_FUNCTION_HEADER
)

if(SKARUPKE_STD_FUNCTION_FOUND)
    add_library(skarupke_std_function INTERFACE)
    target_include_directories(
        skarupke_std_function
        INTERFACE
        ${SKARUPKE_STD_FUNCTION_INCLUDE_DIRS}
    )
    target_compile_definitions(
        skarupke_std_function
        INTERFACE
        SKARUPKE_STD_FUNCTION_FOUND
    )
    add_library(
        skarupke_std_function::skarupke_std_function
        ALIAS
        skarupke_std_function
    )
endif()
