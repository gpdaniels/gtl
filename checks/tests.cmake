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

MESSAGE(STATUS "Checking for tests...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files and test headers.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/source/" "${CMAKE_SOURCE_DIR}/source/*")
FILE(GLOB_RECURSE TEST_FILES RELATIVE "${CMAKE_SOURCE_DIR}/tests/" "${CMAKE_SOURCE_DIR}/tests/*.test.cpp")

# Sort list of files.
LIST(SORT SOURCE_FILES)

# Check if each source file has a test file.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    IF(NOT "${SOURCE_FILE}.test.cpp" IN_LIST TEST_FILES)
        MESSAGE(FATAL_ERROR "Failed to find test file for '${SOURCE_FILE}'.")
    ENDIF()
    
    LIST(REMOVE_ITEM TEST_FILES "${SOURCE_FILE}.test.cpp")
    
ENDFOREACH()

IF(TEST_FILES)
    MESSAGE(FATAL_ERROR "Found more tests than source files: '${TEST_FILES}'")
ENDIF()

MESSAGE(STATUS "Finished test check.")
