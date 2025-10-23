#[[
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
]]

# Define the minimum version of CMake that is required.
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
CMAKE_POLICY(VERSION 3.10)

MESSAGE(STATUS "Checking for clang format ...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})
SET(CMAKE_BINARY_DIR ${BINARY_DIR})

# Get the executable.
FIND_PROGRAM(CLANG_FORMAT_EXECUTABLE NAMES clang-format DOC "clang-format command line tool")
IF(NOT CLANG_FORMAT_EXECUTABLE)
    MESSAGE(STATUS "Failed to find clang-format executable, skipping check.")
    RETURN()
ENDIF()

# Get the version.
EXECUTE_PROCESS(
    COMMAND ${CLANG_FORMAT_EXECUTABLE} --version
    OUTPUT_VARIABLE CLANG_FORMAT_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Verify the version.
STRING(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CLANG_FORMAT_VERSION "${CLANG_FORMAT_VERSION_OUTPUT}")
IF(NOT CLANG_FORMAT_VERSION VERSION_EQUAL "21.1.2")
    MESSAGE(STATUS "Failed to find clang-format version 21.1.2, found: ${CLANG_FORMAT_VERSION}.")
    RETURN()
ENDIF()

# Find source files.
FILE(GLOB_RECURSE CLANG_FORMAT_SOURCES
    RELATIVE ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/source/*
    ${CMAKE_SOURCE_DIR}/tests/*
    ${CMAKE_SOURCE_DIR}/testbench/*
)

SET(FORMAT_ERRORS_FOUND 0)

FOREACH(SOURCE_FILE ${CLANG_FORMAT_SOURCES})
    EXECUTE_PROCESS(
        COMMAND             "${CLANG_FORMAT_EXECUTABLE}" "--dry-run" "-Werror" "-style=file" "-i" "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}"
        WORKING_DIRECTORY   "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY   "${CMAKE_BINARY_DIR}"
        RESULT_VARIABLE     RETURN_CODE
        ERROR_VARIABLE      FORMAT_RESULT
        OUTPUT_QUIET
    )

    # Check for errors running clang-query.
    IF(RETURN_CODE)
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${SOURCE_FILE}:0 (MESSAGE): clang-format tool with error:\n${FORMAT_RESULT}}")
        # Count up the error messages printed.
        MATH(EXPR FORMAT_ERRORS_FOUND "${FORMAT_ERRORS_FOUND}+1")
    ENDIF()
ENDFOREACH()

IF(${FORMAT_ERRORS_FOUND} GREATER 0)
    MESSAGE(FATAL_ERROR "Found ${FORMAT_ERRORS_FOUND} formatting errors.")
ENDIF()

MESSAGE(STATUS "Finished 'clang format' check.")
