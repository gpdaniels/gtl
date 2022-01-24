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

MESSAGE(STATUS "Checking file licenses...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files, test files, and testbench files.
FILE(GLOB_RECURSE LICENSED_FILES RELATIVE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/source/*" "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_SOURCE_DIR}/testbench/*")

# Check each file for the correct license text.
FOREACH(LICENSED_FILE ${LICENSED_FILES})
    
    # Print progress.
    #MESSAGE(STATUS "Processing '${LICENSED_FILE}'...")
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/${LICENSED_FILE}" LICENSED_FILE_CONTENT)
    
    # Replace special list chars.
    STRING(REGEX REPLACE "([[]|[]])" "\\1" GUARDED_FILE_CONTENT "${GUARDED_FILE_CONTENT}")
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" LICENSED_FILE_LINES "${LICENSED_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH LICENSED_FILE_LINES LICENSED_FILE_LENGTH)
    IF(LICENSED_FILE_LENGTH LESS 18)
        MESSAGE(FATAL_ERROR "License in file '${LICENSED_FILE}' does not match: The file is too short.")
    ENDIF()

    SET(YEARS_REGEX "(2018)|(2019)|(2020)|(2021)|(2022)")
    
    # Prepare the license regex.
    SET(LICENSE_REGEX_LINES
        "^/\\*$"
        "^The MIT License$"
        "^Copyright \\(c\\) ${YEARS_REGEX} Geoffrey Daniels\\. http://gpdaniels\\.com/$"
        "^Permission is hereby granted, free of charge, to any person obtaining a copy$"
        "^of this software and associated documentation files \\(the \"Software\"\\), to deal$"
        "^in the Software without restriction, including without limitation the rights$"
        "^to use, copy, modify, merge, publish, distribute, sublicense, and/or sell$"
        "^copies of the Software, and to permit persons to whom the Software is$"
        "^furnished to do so, subject to the following conditions:$"
        "^The above copyright notice and this permission notice shall be included in$"
        "^all copies or substantial portions of the Software\\.$"
        "^THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR$"
        "^IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,$"
        "^FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT\\. IN NO EVENT SHALL THE$"
        "^AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER$"
        "^LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,$"
        "^OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN$"
        "^THE SOFTWARE$"
        "^\\*/$"
    )
    
    # Check each line.
    FOREACH(LICENSE_LINE 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18)
        LIST(GET LICENSED_FILE_LINES ${LICENSE_LINE} LICENSED_FILE_LINE)
        LIST(GET LICENSE_REGEX_LINES ${LICENSE_LINE} LICENSE_REGEX_LINE)
        IF(NOT LICENSED_FILE_LINE MATCHES "${LICENSE_REGEX_LINE}")
            MATH(EXPR LICENSE_LINE "${LICENSE_LINE}+1")
            MESSAGE(FATAL_ERROR "License in file '${LICENSED_FILE}' does not match on line ${LICENSE_LINE}:\n'${LICENSED_FILE_LINE}' != '${LICENSE_REGEX_LINE}'.")
        ENDIF()
    ENDFOREACH()
    
ENDFOREACH()

MESSAGE(STATUS "Finished file licenses check.")

