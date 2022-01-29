#
# Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3 of the License only.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Define the minimum version of CMake that is required.
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
CMAKE_POLICY(VERSION 3.5.1)

MESSAGE(STATUS "Checking file licenses...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test files, and testbench files.
FILE(GLOB_RECURSE LICENSED_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_SOURCE_DIR}/testbench/*")

# Sort list of files.
LIST(SORT LICENSED_FILES)

# Check each file for the correct license text.
FOREACH(LICENSED_FILE ${LICENSED_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${LICENSED_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${LICENSED_FILE}" LICENSED_FILE_CONTENT)
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" LICENSED_FILE_LINES "${LICENSED_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH LICENSED_FILE_LINES LICENSED_FILE_LENGTH)
    IF(LICENSED_FILE_LENGTH LESS 18)
        MESSAGE(FATAL_ERROR "License in file '${LICENSED_FILE}' does not match: The file is too short.")
    ENDIF()

    # Prepare the license regex.
    SET(LICENSE_REGEX_LINES
        "^/\\*$"
        "^Copyright \\(C\\) 2018-2022 Geoffrey Daniels\\. http://gpdaniels\\.com/$"
        "^$"
        "^This program is free software: you can redistribute it and/or modify$"
        "^it under the terms of the GNU General Public License as published by$"
        "^the Free Software Foundation, version 3 of the License only\\.$"
        "^$"
        "^This program is distributed in the hope that it will be useful,$"
        "^but WITHOUT ANY WARRANTY; without even the implied warranty of$"
        "^MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\\.  See the$"
        "^GNU General Public License for more details\\.$"
        "^$"
        "^You should have received a copy of the GNU General Public License$"
        "^along with this program\\.  If not, see <https://www\\.gnu\\.org/licenses/>.$"
        "^\\*/$"
    )
    
    # Check each line.
    FOREACH(LICENSE_LINE 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
        LIST(GET LICENSED_FILE_LINES ${LICENSE_LINE} LICENSED_FILE_LINE)
        LIST(GET LICENSE_REGEX_LINES ${LICENSE_LINE} LICENSE_REGEX_LINE)
        IF(NOT LICENSED_FILE_LINE MATCHES "${LICENSE_REGEX_LINE}")
            MATH(EXPR LICENSE_LINE "${LICENSE_LINE}+1")
            MESSAGE(FATAL_ERROR "License in file '${LICENSED_FILE}' does not match on line ${LICENSE_LINE}:\n'${LICENSED_FILE_LINE}' != '${LICENSE_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
ENDFOREACH()

MESSAGE(STATUS "Finished file licenses check.")

