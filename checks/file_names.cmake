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

MESSAGE(STATUS "Checking file names...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all project files.
FILE(GLOB_RECURSE PROJECT_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/*")

# Ignore git and build directories.
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^.git/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^[Bb][Uu][Ii][Ll][Dd]/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^#.*$")

# Sort list of files.
LIST(SORT PROJECT_FILES)

# Check if each filename is lowercase.
FOREACH(PROJECT_FILE ${PROJECT_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${PROJECT_FILE}'...")
    
    # Special files.
    IF("${PROJECT_FILE}" MATCHES "^(CMakeLists.txt)|(CODE_OF_CONDUCT.md)|(LICENSE)|(README.md)|(.github/.+)$")
        CONTINUE()
    ENDIF()
    
    # Root hidden files.
    IF("${PROJECT_FILE}" MATCHES "^[.][a-z]+([.][a-z]+)?$")
    # Root files.
    ELSEIF("${PROJECT_FILE}" MATCHES "^[a-z]+[a-z_0-9]*[.][a-z]+$")
    # Check files.
    ELSEIF("${PROJECT_FILE}" MATCHES "^checks/[a-z]+[a-z_0-9]*[.]cmake$")
    # Source files.
    ELSEIF("${PROJECT_FILE}" MATCHES "^source/([a-z]+[a-z_0-9]*/)+[a-z]+[a-z_0-9]*$")
    # Test files.
    ELSEIF("${PROJECT_FILE}" MATCHES "^tests/([a-z]+[a-z_0-9]*/)+[a-z]+[a-z_0-9]*[.]test[.][ch]pp$")
    # Testbench files.
    ELSEIF("${PROJECT_FILE}" MATCHES "^testbench/[a-z]+[a-z_0-9]*[.]tests[.][ch]pp$")
    # Otherwise.
    ELSE()
        MESSAGE(FATAL_ERROR "Found invalid file name: '${PROJECT_FILE}'")
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished file name check.")
