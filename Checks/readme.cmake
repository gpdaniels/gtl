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

MESSAGE(STATUS "Checking coverage of README.md file...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/Source/" "${CMAKE_SOURCE_DIR}/Source/*")

# Get README.md content.
FILE(READ "${CMAKE_SOURCE_DIR}/README.md" README_FILE_CONTENT)

# Replace special list chars.
STRING(REGEX REPLACE "([[]|[]])" "\\\\1" README_FILE_CONTENT "${README_FILE_CONTENT}")

# Make the contents lower case.
STRING(TOLOWER "${README_FILE_CONTENT}" README_FILE_CONTENT)

# Check each source file has been documented in the README.md file.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    # Extract file name and group.
    GET_FILENAME_COMPONENT(SOURCE_FILE_NAME ${SOURCE_FILE} NAME)
    GET_FILENAME_COMPONENT(SOURCE_FILE_GROUP ${SOURCE_FILE} DIRECTORY)
    
    # If the group is not found raise an error.
    IF(NOT README_FILE_CONTENT MATCHES "###[ ]${SOURCE_FILE_GROUP}[ ]###")
        MESSAGE(FATAL_ERROR "Failed to find group ${SOURCE_FILE_GROUP} in README.md file.")
    ENDIF()
    
    # If the file name is not found raise an error.
    IF(NOT README_FILE_CONTENT MATCHES "\\|[ ]*\\*\\*${SOURCE_FILE_NAME}\\*\\*[ ]\\|")
        MESSAGE(FATAL_ERROR "Failed to find file ${SOURCE_FILE_NAME} in README.md file.")
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished coverage of README.md file check.")
