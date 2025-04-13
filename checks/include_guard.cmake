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

MESSAGE(STATUS "Checking include guards...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test headers, and testbench headers.
FILE(GLOB_RECURSE GUARDED_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*.hpp" "${CMAKE_SOURCE_DIR}/testbench/*.hpp")

# Sort list of files.
LIST(SORT GUARDED_FILES)

# Create an empty database to fill with all the include guard defines.
SET(GUARD_DATABASE "")
SET(HEADER_DATABASE "")

# Check each file for include guard correctness.
FOREACH(GUARDED_FILE ${GUARDED_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${GUARDED_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${GUARDED_FILE}" GUARDED_FILE_CONTENT)
    
    # Replace special list chars.
    STRING(REGEX REPLACE "([[]|[]])" "\\\\\\\\1" GUARDED_FILE_CONTENT "${GUARDED_FILE_CONTENT}")
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" GUARDED_FILE_LINES "${GUARDED_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH GUARDED_FILE_LINES GUARDED_FILE_LENGTH)
    IF(GUARDED_FILE_LENGTH LESS 20)
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${GUARDED_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Include guard in file '${GUARDED_FILE}' is not correct: The file is too short.")
    ENDIF()
    
    # Calculate the end guard lines location.
    MATH(EXPR GUARDED_FILE_LENGTH "${GUARDED_FILE_LENGTH}-2")
    
    # Create a new list from the file lines.
    LIST(GET GUARDED_FILE_LINES 17 18 19 ${GUARDED_FILE_LENGTH} GUARDED_FILE_GUARD_LINES)

    # Determine the include guard name from the file name.
    SET(GUARDED_FILE_NAME "${GUARDED_FILE}")
    STRING(REGEX REPLACE "\\.hpp$" "" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(REGEX REPLACE "^source/" "" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(REGEX REPLACE "^tests/" "" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(REGEX REPLACE "^testbench/" "" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(REPLACE "." "_" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(REPLACE "/" "_" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(TOUPPER "${GUARDED_FILE_NAME}" GUARDED_FILE_NAME)
    
    # Prepare the guard regex.
    SET(GUARD_REGEX_LINES
        "#pragma once"
        "#ifndef GTL_${GUARDED_FILE_NAME}_HPP"
        "#define GTL_${GUARDED_FILE_NAME}_HPP"
        "#endif // GTL_${GUARDED_FILE_NAME}_HPP"
    )
    
    # Check each line.
    FOREACH(GUARD_LINE 0 1 2 3)
        LIST(GET GUARDED_FILE_GUARD_LINES ${GUARD_LINE} GUARDED_FILE_GUARD_LINE)
        LIST(GET GUARD_REGEX_LINES ${GUARD_LINE} GUARD_REGEX_LINE)
        IF(NOT GUARDED_FILE_GUARD_LINE MATCHES "${GUARD_REGEX_LINE}")
            MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${GUARDED_FILE}:0 (MESSAGE):")
            MESSAGE(FATAL_ERROR "Include guard in file '${GUARDED_FILE}' is not correct:\n'${GUARDED_FILE_GUARD_LINE}' != '${GUARD_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
    # Check the guard is unique.
    LIST(FIND GUARD_DATABASE "GTL_${GUARDED_FILE_NAME}_HPP" INDEX_DATABASE)
    IF(${INDEX_DATABASE} GREATER -1)
        LIST(GET HEADER_DATABASE ${INDEX_DATABASE} DUPLICATE_FILE)
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${GUARDED_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Include guard in file '${GUARDED_FILE}' is not unique, it matches '${DUPLICATE_FILE}'.")
    ENDIF()
    LIST(APPEND GUARD_DATABASE "GTL_${GUARDED_FILE_NAME}_HPP")
    LIST(APPEND HEADER_DATABASE "${GUARDED_FILE}")
    
ENDFOREACH()

MESSAGE(STATUS "Finished include guards check.")
