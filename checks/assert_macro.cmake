#
# The MIT License
# Copyright (c) 2020 Geoffrey Daniels. http://gpdaniels.com/
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

MESSAGE(STATUS "Checking assert macros...")   

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*")

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
        "^#   if defined\\(_MSC_VER\\)$"
        "^#       define __builtin_trap\\(\\) __debugbreak\\(\\)$"
        "^#   endif$"
        "^/// @brief A simple assert macro to break the program if the ${SOURCE_FILE_NAME} is misused\\.$"
        "^#   define GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\\(ASSERTION, MESSAGE\\) static_cast<void>\\(\\(ASSERTION\\) || \\(__builtin_trap(), 0\\)\\)$"
        "^#else$"
        "^/// @brief At release time the assert macro is implemented as a nop\\.$"
        "^#   define GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\\(ASSERTION, MESSAGE\\) static_cast<void>\\(0\\)$"
        "^#endif$"
        "^$"
    )
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" SOURCE_FILE_LINES "${SOURCE_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH SOURCE_FILE_LINES SOURCE_FILE_LENGTH)
    IF(SOURCE_FILE_LENGTH LESS 35)
        MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is not correct: The file is too short.")
    ENDIF()
    
    # Create a new list from the file lines.
    LIST(GET SOURCE_FILE_LINES 26 27 28 29 30 31 32 33 34 35 SOURCE_FILE_ASSERT_LINES)
    
    # Check each line.
    FOREACH(ASSERT_LINE 0 1 2 3 4 5 6 7 8 9)
        LIST(GET SOURCE_FILE_ASSERT_LINES ${ASSERT_LINE} SOURCE_FILE_ASSERT_LINE)
        LIST(GET ASSERT_REGEX_LINES ${ASSERT_LINE} ASSERT_REGEX_LINE)
        IF(NOT SOURCE_FILE_ASSERT_LINE MATCHES "${ASSERT_REGEX_LINE}")
            MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is not correct:\n'${SOURCE_FILE_ASSERT_LINE}' != '${ASSERT_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
    # Check there is an undef at the end.
    IF(NOT SOURCE_FILE_CONTENT MATCHES "\n#undef GTL_${SOURCE_FILE_NAME_UPPER}_ASSERT\n")
        MESSAGE(FATAL_ERROR "Assert macro in file '${SOURCE_FILE}' is missing its undef.")
    ENDIF()
    
ENDFOREACH()

MESSAGE(STATUS "Finished assert macros check.")
