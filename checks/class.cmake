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
    
    # If there is no definition then error.
    IF(NOT CLASS_FILE_CONTENT MATCHES "class ${CLASS_FILE_NAME}")
        IF(NOT CLASS_FILE_CONTENT MATCHES "namespace ${CLASS_FILE_NAME}")
            STRING(TOUPPER "${CLASS_FILE_NAME}" CLASS_FILE_NAME_UPPER)
            IF(NOT CLASS_FILE_CONTENT MATCHES "#define GTL_${CLASS_FILE_NAME_UPPER}")
                # TODO: Convert this warning into a FATAL_ERROR.
                MESSAGE(STATUS "Missing definition of class, namespace, or #define including '${CLASS_FILE_NAME}' in ${CLASS_FILE}.")
            ENDIF()
        ENDIF()
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished classes check.")
