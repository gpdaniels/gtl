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

MESSAGE(STATUS "Checking for merge conflicts...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all project files.
FILE(GLOB_RECURSE PROJECT_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/*")

# Ignore git and build directories.
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^.git/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^[Bb][Uu][Ii][Ll][Dd]/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^#.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^wip/.*$")
LIST(FILTER PROJECT_FILES EXCLUDE REGEX "^checks/merge_conflicts.cmake$")

# Sort list of files.
LIST(SORT PROJECT_FILES)

# Check each file for the correct license text.
FOREACH(PROJECT_FILE ${PROJECT_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${PROJECT_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${PROJECT_FILE}" PROJECT_FILE_CONTENT)
    
    # Search for merge conflicts markers.
    STRING(FIND "${PROJECT_FILE_CONTENT}" "<<<<<<<" FOUND_MERGE_CONFLICT_LOCATION)
    
    # If any are found the match location will be something other than -1.
    IF(NOT "${FOUND_MERGE_CONFLICT_LOCATION}" MATCHES "-1")
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Found a merge conflict in file '${PROJECT_FILE}'.")
    ENDIF()
    
    # Search for merge conflicts markers.
    STRING(FIND "${PROJECT_FILE_CONTENT}" "=======" FOUND_MERGE_CONFLICT_LOCATION)
    
    # If any are found the match location will be something other than -1.
    IF(NOT "${FOUND_MERGE_CONFLICT_LOCATION}" MATCHES "-1")
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Found a merge conflict in file '${PROJECT_FILE}'.")
    ENDIF()
    
    # Search for merge conflicts markers.
    STRING(FIND "${PROJECT_FILE_CONTENT}" ">>>>>>>" FOUND_MERGE_CONFLICT_LOCATION)
    
    # If any are found the match location will be something other than -1.
    IF(NOT "${FOUND_MERGE_CONFLICT_LOCATION}" MATCHES "-1")
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Found a merge conflict in file '${PROJECT_FILE}'.")
    ENDIF()
        
ENDFOREACH()

MESSAGE(STATUS "Finished merge conflict check.")

