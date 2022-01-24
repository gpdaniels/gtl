#
# The MIT License
# Copyright (c) 2020 Geoffrey Daniels. http://gpdaniels.com/
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE
#

# Define the minimum version of CMake that is required.
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
CMAKE_POLICY(VERSION 3.5.1)

MESSAGE(STATUS "Checking include guards...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test headers, and testbench headers.
FILE(GLOB_RECURSE GUARDED_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*.hpp" "${CMAKE_SOURCE_DIR}/testbench/*.hpp")

# Check each file for include guard correctness.
FOREACH(GUARDED_FILE ${GUARDED_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${GUARDED_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${GUARDED_FILE}" GUARDED_FILE_CONTENT)
    
    # Replace special list chars.
    STRING(REGEX REPLACE "([[]|[]])" "\\1" GUARDED_FILE_CONTENT "${GUARDED_FILE_CONTENT}")
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" GUARDED_FILE_LINES "${GUARDED_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH GUARDED_FILE_LINES GUARDED_FILE_LENGTH)
    IF(GUARDED_FILE_LENGTH LESS 24)
        MESSAGE(FATAL_ERROR "Include guard in file '${GUARDED_FILE}' is not correct: The file is too short.")
    ENDIF()

    # Calculate the end guard lines location.
    MATH(EXPR GUARDED_FILE_LENGTH "${GUARDED_FILE_LENGTH}-2")
    
    # Create a new list from the file lines.
    LIST(GET GUARDED_FILE_LINES 20 21 22 ${GUARDED_FILE_LENGTH} GUARDED_FILE_GUARD_LINES)
    
    # Determine the include guard name from the file name.
    GET_FILENAME_COMPONENT(GUARDED_FILE_NAME "${GUARDED_FILE}" NAME)
    STRING(REGEX REPLACE "\\.hpp$" "" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    STRING(TOUPPER "${GUARDED_FILE_NAME}" GUARDED_FILE_NAME)
    STRING(REPLACE "." "_" GUARDED_FILE_NAME "${GUARDED_FILE_NAME}")
    
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
            MESSAGE(FATAL_ERROR "Include guard in file '${GUARDED_FILE}' is not correct:\n'${GUARDED_FILE_GUARD_LINE}' != '${GUARD_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
ENDFOREACH()

MESSAGE(STATUS "Finished include guards check.")
