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

MESSAGE(STATUS "Checking assert macros...")   

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*")

# Sort list of files.
LIST(SORT SOURCE_FILES)

# Check each file for assert macros.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}" SOURCE_FILE_CONTENT)
    
    # Determine the include guard name of the file.
    GET_FILENAME_COMPONENT(SOURCE_FILE_NAME "${SOURCE_FILE}" NAME)
    STRING(TOUPPER "${SOURCE_FILE_NAME}" SOURCE_FILE_NAME_UPPER)
    
    # If there is no assert macro, then skip.
    IF(NOT SOURCE_FILE_CONTENT MATCHES "GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT")
        CONTINUE()
    ENDIF()
    
    # Prepare the assert macro regex.
    SET(ASSERT_REGEX_LINES
        "^#ifndef NDEBUG$"
        "^#if defined\\(_MSC_VER\\)$"
        "^#define __builtin_trap\\(\\) __debugbreak\\(\\)$"
        "^#endif$"
        "^/// @brief A simple assert macro to break the program if the ${SOURCE_FILE_NAME} is misused\\.$"
        "^#define GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\\(ASSERTION, MESSAGE\\) static_cast<void>\\(\\(ASSERTION\\) || \\(__builtin_trap(), 0\\)\\)$"
        "^#else$"
        "^/// @brief At release time the assert macro is implemented as a nop\\.$"
        "^#define GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\\(ASSERTION, MESSAGE\\) static_cast<void>\\(0\\)$"
        "^#endif$"
    )
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" SOURCE_FILE_LINES "${SOURCE_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH SOURCE_FILE_LINES SOURCE_FILE_LENGTH)
    IF(SOURCE_FILE_LENGTH LESS 35)
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${SOURCE_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is not correct: The file is too short.")
    ENDIF()
    
    # Create a new list from the file lines.
    LIST(GET SOURCE_FILE_LINES 23 24 25 26 27 28 29 30 31 32 SOURCE_FILE_ASSERT_LINES)
    
    # Check each line.
    FOREACH(ASSERT_LINE 0 1 2 3 4 5 6 7 8 9)
        LIST(GET SOURCE_FILE_ASSERT_LINES ${ASSERT_LINE} SOURCE_FILE_ASSERT_LINE)
        LIST(GET ASSERT_REGEX_LINES ${ASSERT_LINE} ASSERT_REGEX_LINE)
        IF(NOT SOURCE_FILE_ASSERT_LINE MATCHES "${ASSERT_REGEX_LINE}")
            MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${SOURCE_FILE}:0 (MESSAGE):")
            MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is not correct:\n'${SOURCE_FILE_ASSERT_LINE}' != '${ASSERT_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
    # Check there is an undef at the end.
    IF(NOT SOURCE_FILE_CONTENT MATCHES "\n#undef GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\n")
        MESSAGE("CMake Error at ${CMAKE_SOURCE_DIR}/${SOURCE_FILE}:0 (MESSAGE):")
        MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is missing its undef.")
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished assert macros check.")
