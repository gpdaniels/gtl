#
# The MIT License
# Copyright (c) 2021 Geoffrey Daniels. http://gpdaniels.com/
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

MESSAGE(STATUS "Checking for tabs...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all project files.
FILE(GLOB_RECURSE PROJECT_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/*")

# Ignore git and build directories.
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^.git/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^[Bb][Uu][Ii][Ll][Dd]/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^#.*$")

# Check each file for the correct license text.
FOREACH(PROJECT_FILE ${PROJECT_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${PROJECT_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${PROJECT_FILE}" PROJECT_FILE_CONTENT)
    
    # Replace special list chars.
    STRING(REGEX REPLACE "([[]|[]])" "\\\\1" PROJECT_FILE_CONTENT "${PROJECT_FILE_CONTENT}")
    
    # Search for tab characters.
    STRING(FIND "${PROJECT_FILE_CONTENT}" "\t" FOUND_TAB_LOCATION)
    
    # If any are found the match location will be something other than -1.
    IF(NOT "${FOUND_TAB_LOCATION}" MATCHES "-1")
        MESSAGE(FATAL_ERROR "Found a tab in file '${PROJECT_FILE}'.")
    ENDIF()
        
ENDFOREACH()

MESSAGE(STATUS "Finished tab check.")

