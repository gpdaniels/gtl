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

MESSAGE(STATUS "Checking classes...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE CLASS_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*")

# Sort list of files.
LIST(SORT CLASS_FILES)

# Check each file for include guard correctness.
FOREACH(CLASS_FILE ${CLASS_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${CLASS_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${CLASS_FILE}" CLASS_FILE_CONTENT)
    
    # Determine the class name from the file name.
    GET_FILENAME_COMPONENT(CLASS_FILE_NAME "${CLASS_FILE}" NAME)
    
    # If there is no class definition then error.
    IF(NOT CLASS_FILE_CONTENT MATCHES "class ${CLASS_FILE_NAME}")
        IF(NOT CLASS_FILE_CONTENT MATCHES "namespace ${CLASS_FILE_NAME}")
            # TODO: Convert this warning into a FATAL_ERROR.
            MESSAGE(STATUS "Missing definition of 'class ${CLASS_FILE_NAME}' or 'namespace ${CLASS_FILE_NAME}' in ${CLASS_FILE}.")
        ENDIF()
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished classes check.")
