#[[
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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
