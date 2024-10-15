# MIT License
#
# Copyright (c) 2024 Subhadip Roy Chowdhury
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.13)
project(cstd C)
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
option(NO_DEFAULT_FUNCTION_IMPLEMENTATIONS "Do not add default function implementations." ON)
option(NO_TESTS "Do not enable tests." OFF)
option(DEBUG_MODE "Enable debugging mode." OFF)
add_compile_options(-O3 -g -pipe -march=x86-64 -mtune=generic -shared -Wall -flto=auto -fdiagnostics-color=always -Wno-pointer-to-int-cast -fsanitize=address -fPIC -Werror)
if (NOT NO_DEFAULT_FUNCTION_IMPLEMENTATIONS)
	add_compile_definitions(CSTD_NO_DEFAULT_FUNCTION_IMPLEMENTATIONS)
endif()
if (DEBUG_MODE)
	add_compile_definitions(CSTD_DEBUG_MODE)
endif()
add_library(cstd_static STATIC ${SOURCE_FILES})
add_library(cstd SHARED ${SOURCE_FILES})
target_include_directories(cstd_static PRIVATE include/)
target_include_directories(cstd PRIVATE include/)
set_target_properties(cstd_static PROPERTIES OUTPUT_NAME "cstd")
set_target_properties(cstd PROPERTIES OUTPUT_NAME "cstd")
set_target_properties(cstd_static PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
set_target_properties(cstd PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# tests
if(NOT NO_TESTS)
	file(GLOB TEST_FILES "${CMAKE_SOURCE_DIR}/tests/*.c")
	foreach(TEST_FILE ${TEST_FILES})
		get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
		add_executable(${TEST_NAME} ${TEST_FILE})
		target_compile_options(${TEST_NAME} PRIVATE -O3 -g -pipe -march=x86-64 -mtune=generic -Wall -flto=auto -fdiagnostics-color=always -fsanitize=address -fPIC -Werror)
		target_include_directories(${TEST_NAME} PRIVATE include/)
		target_link_libraries(${TEST_NAME} cstd_static)
	endforeach()
endif()