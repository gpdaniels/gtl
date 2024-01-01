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
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
CMAKE_POLICY(VERSION 3.5.1)

MESSAGE(STATUS "Checking for 'using namespace'...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*")

# Sort list of files.
LIST(SORT SOURCE_FILES)

# Check each file for include guard correctness.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}" SOURCE_FILE_CONTENT)
    
    # If 'using namespace' is detected raise an error.
    IF(SOURCE_FILE_CONTENT MATCHES "using namespace")
        MESSAGE(FATAL_ERROR "Detected 'using namespace' in ${SOURCE_FILE}.")
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished 'using namespace' check.")
