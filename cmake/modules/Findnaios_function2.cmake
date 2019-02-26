if(TARGET naios_function2)
    return()
endif()

find_file(
    NAIOS_FUNCTION2_HEADER
    "/third_party/function_libraries/naios/function2/include/function2/function2.hpp"
    ${CMAKE_SOURCE_DIR}
)
if(NOT NAIOS_FUNCTION2_HEADER STREQUAL "NAIOS_FUNCTION2_HEADER-NOTFOUND")
    set(
        NAIOS_FUNCTION2_INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/function_libraries/naios/function2/include"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    naios_function2
    DEFAULT_MSG
    NAIOS_FUNCTION2_INCLUDE_DIRS
    NAIOS_FUNCTION2_HEADER
)

if(NAIOS_FUNCTION2_FOUND)
    add_library(naios_function2 INTERFACE)
    target_include_directories(
        naios_function2
        INTERFACE
        ${NAIOS_FUNCTION2_INCLUDE_DIRS}
    )
    target_compile_definitions(
        naios_function2
        INTERFACE
        NAIOS_FUNCTION2_FOUND
    )
    add_library(
        naios_function2::naios_function2
        ALIAS
        naios_function2
    )
endif()
