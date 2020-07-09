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

MESSAGE(STATUS "Checking include guards...")

# Find all source files, test headers, and testbench headers.
FILE(GLOB_RECURSE GUARDED_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/Source/*" "${CMAKE_SOURCE_DIR}/Tests/*.hpp" "${CMAKE_SOURCE_DIR}/Testbench/*.hpp")

# Check each file for include guard correctness.
FOREACH(GUARDED_FILE ${GUARDED_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${GUARDED_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${GUARDED_FILE}" GUARDED_FILE_CONTENT)
    
    # Replace special list chars.
    STRING(REGEX REPLACE "([[]|[]])" "\\\\1" GUARDED_FILE_CONTENT "${GUARDED_FILE_CONTENT}")
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" GUARDED_FILE_LINES "${GUARDED_FILE_CONTENT}")
    
    # Calculate the end guard lines location.
    LIST(LENGTH GUARDED_FILE_LINES GUARDED_FILE_LENGTH)
    MATH(EXPR GUARDED_FILE_LENGTH "${GUARDED_FILE_LENGTH}-2")
    
    # Create a new list from the file lines.
    LIST(GET GUARDED_FILE_LINES 20 21 22 ${GUARDED_FILE_LENGTH} GUARDED_FILE_GUARD_LINES)
    
    # Determine the include guard name of the file.
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
