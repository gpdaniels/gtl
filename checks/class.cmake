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
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
CMAKE_POLICY(VERSION 3.10)

MESSAGE(STATUS "Checking classes...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE CLASS_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*")

# Sort list of files.
LIST(SORT CLASS_FILES)

SET(MISSING_DEFINITIONS_FOUND 0)
SET(MISSING_DEFINITIONS_MAX 5)

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
                MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${CLASS_FILE}:0 (MESSAGE): Missing definition of class, namespace, or #define including '${CLASS_FILE_NAME}'.")
                # Count up the error messages printed.
                MATH(EXPR MISSING_DEFINITIONS_FOUND "${MISSING_DEFINITIONS_FOUND}+1")
            ENDIF()
        ENDIF()
    ENDIF()

    # Limit the max number of errors printed.
    IF(${MISSING_DEFINITIONS_FOUND} GREATER ${MISSING_DEFINITIONS_MAX})
        MESSAGE("CMake Error at ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} (MESSAGE): Found more than ${MISSING_DEFINITIONS_MAX} missing definitions. Skipping printing.")
        BREAK()
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished classes check.")
