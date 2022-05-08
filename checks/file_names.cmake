#[[
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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
