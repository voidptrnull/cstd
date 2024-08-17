cmake_minimum_required(VERSION 3.13)
project(c-utils C)
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
option(NO_COMP_FUNCTIONS "Do not add default compare functions." ON)
add_compile_options(-O3 -Wall -flto=auto -fdiagnostics-color=always -fPIC -Werror)
if (NOT NO_COMP_FUNCTIONS)
    add_compile_definitions(CUTILS_COMP_FUNCTIONS)
endif()
add_library(cutils_static STATIC ${SOURCE_FILES})
add_library(cutils SHARED ${SOURCE_FILES})
target_include_directories(cutils_static PRIVATE include/)
target_include_directories(cutils PRIVATE include/)
set_target_properties(cutils_static PROPERTIES OUTPUT_NAME "cutils")
set_target_properties(cutils PROPERTIES OUTPUT_NAME "cutils")
set_target_properties(cutils_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
set_target_properties(cutils PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
