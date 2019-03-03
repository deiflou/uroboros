cmake_minimum_required(VERSION 3.1.0)

project(uroboros VERSION 0.1.0)

include(GNUInstallDirs)
include("cmake/utilities.cmake")

set(CMAKE_CXX_STANDARD 17)
set(UROBOROS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
if(WIN32)
    set(UROBOROS_WIN32_EXECUTABLE WIN32)
endif()

add_library(uroboros INTERFACE)
target_include_directories(
    uroboros
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
add_library(uroboros::uroboros ALIAS uroboros)

install(TARGETS uroboros EXPORT uroborosConfig)
install(EXPORT uroborosConfig DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/uroboros")
install(DIRECTORY include/uroboros DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(examples)
add_subdirectory(docs)
