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

MESSAGE(STATUS "Checking todo comments...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test files, and testbench files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/checks/*" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_SOURCE_DIR}/testbench/*")

# Sort list of files.
LIST(SORT SOURCE_FILES)

SET(TODOS_COMMENTS_FOUND 0)
SET(TODOS_COMMENTS_MAX 5)

# Check each file for the correct license text.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}" SOURCE_FILE_CONTENT)
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" SOURCE_FILE_LINES "${SOURCE_FILE_CONTENT}")
    
    # Remove all lines that don't match the TODO regex.
    LIST(FILTER SOURCE_FILE_LINES INCLUDE REGEX "^[ ]*(([#])|([/][/]))[ ]*[Tt][Oo][Dd][Oo][: ]")
    
    # Print a message for each found TODO comment..
    FOREACH(SOURCE_FILE_LINE ${SOURCE_FILE_LINES})
        STRING(STRIP ${SOURCE_FILE_LINE} SOURCE_FILE_LINE)
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${SOURCE_FILE}:0 (MESSAGE): Found TODO: '${SOURCE_FILE_LINE}'.")
        # Count up the error messages printed.
        MATH(EXPR TODOS_COMMENTS_FOUND "${TODOS_COMMENTS_FOUND}+1")
        # There can be more than one TODO per file, so early exit the internal loop if we are at the limit.
        IF(${TODOS_COMMENTS_FOUND} GREATER ${TODOS_COMMENTS_MAX})
            BREAK()
        ENDIF()
    ENDFOREACH()

    # Limit the max number of errors printed.
    IF(${TODOS_COMMENTS_FOUND} GREATER ${TODOS_COMMENTS_MAX})
        MESSAGE("CMake Error at ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} (MESSAGE): Found more than ${TODOS_COMMENTS_MAX} TODO comments. Skipping printing.")
        BREAK()
    ENDIF()
ENDFOREACH()

MESSAGE(STATUS "Finished todo comments check.")

