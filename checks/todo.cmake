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

MESSAGE(STATUS "Checking todo comments...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test files, and testbench files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/checks/*" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_SOURCE_DIR}/testbench/*")

# Sort list of files.
LIST(SORT SOURCE_FILES)

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
    
    # Check each file for the correct license text.
    FOREACH(SOURCE_FILE_LINE ${SOURCE_FILE_LINES})
        STRING(STRIP ${SOURCE_FILE_LINE} SOURCE_FILE_LINE)
        MESSAGE(STATUS "Found TODO in '${SOURCE_FILE}': '${SOURCE_FILE_LINE}'.")
    ENDFOREACH()
    
ENDFOREACH()

MESSAGE(STATUS "Finished todo comments check.")

