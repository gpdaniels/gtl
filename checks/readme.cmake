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

MESSAGE(STATUS "Checking coverage of README.md file...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Find all source files.
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}/source/" "${CMAKE_SOURCE_DIR}/source/*")

# Sort list of files.
LIST(SORT SOURCE_FILES)

# Prepare a list to hold the lines of the readme table.
SET(README_TABLE_LINES "")

# Add the table header.
LIST(APPEND README_TABLE_LINES "| Group      |                Class | Description                                                    | :grey_question: |")
LIST(APPEND README_TABLE_LINES "|:-----------|---------------------:|:---------------------------------------------------------------|:---------------:|")

# Generate the table contents.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    # Print progress.
    #MESSAGE(STATUS "Processing '${SOURCE_FILE}'...")
    
    # Extract file name and group.
    GET_FILENAME_COMPONENT(SOURCE_FILE_NAME ${SOURCE_FILE} NAME)
    GET_FILENAME_COMPONENT(SOURCE_FILE_GROUP ${SOURCE_FILE} DIRECTORY)
    
    # Get content.
    FILE(READ "${CMAKE_SOURCE_DIR}/source/${SOURCE_FILE}" SOURCE_FILE_CONTENT)
    
    # Replace newlines.
    STRING(REGEX REPLACE "[\r]?[\n]" ";" SOURCE_FILE_LINES "${SOURCE_FILE_CONTENT}")
    
    # Get the number of lines in the file.
    LIST(LENGTH SOURCE_FILE_LINES SOURCE_FILE_LENGTH)
    IF(SOURCE_FILE_LENGTH LESS 24)
        MESSAGE(FATAL_ERROR "Readme description line in file '${SOURCE_FILE}' is not correct: The file is too short.")
    ENDIF()
    
    # Extract description from file.
    LIST(GET SOURCE_FILE_LINES 21 DESCRIPTION_LINE)
    
    # Validate description line.
    SET(DESCRIPTION_LINE_REGEX "^// Summary:([ ]([A-Za-z][A-Za-z0-9 -_.,]+[.]))?([ ](\\[[Ww][Ii][Pp]\\]))?$")
    IF(NOT DESCRIPTION_LINE MATCHES "${DESCRIPTION_LINE_REGEX}")
        MESSAGE(FATAL_ERROR "Readme description line in file '${SOURCE_FILE}' is invalid.")
    ENDIF()
    
    # Validate and parse the description line.
    STRING(REGEX MATCH "^// Summary:([ ]([A-Za-z][A-Za-z0-9 -_.,]+[.]))?([ ](\\[[Ww][Ii][Pp]\\]))?$" DESCRIPTION_CELLS "${DESCRIPTION_LINE}")
    
    # Extract the summary and wip status.
    SET(DESCRIPTION_CELL "${CMAKE_MATCH_2}")
    SET(COMPLETION_CELL "${CMAKE_MATCH_3}")
    
    # Escape markdown in the description.
    STRING(REGEX REPLACE "([_-])" "\\\\\\1" DESCRIPTION_CELL "${DESCRIPTION_CELL}")
    
    # Convert the wip status into an emoji.
    IF(NOT COMPLETION_CELL MATCHES "\\[[Ww][Ii][Pp]\\]")
        SET(COMPLETION_CELL ":heavy_check_mark:")
    ELSE()
        SET(COMPLETION_CELL ":construction:")
    ENDIF()
    
    # Append the table row for this file.
    LIST(APPEND README_TABLE_LINES "| [${SOURCE_FILE_GROUP}](source/${SOURCE_FILE_GROUP}) | [${SOURCE_FILE_NAME}](source/${SOURCE_FILE_GROUP}/${SOURCE_FILE_NAME}) | ${DESCRIPTION_CELL} | ${COMPLETION_CELL} |")
    
ENDFOREACH()

# Dump the table to file.
STRING(REGEX REPLACE ";" "\n" TABLE_CONTENT "${README_TABLE_LINES}")
FILE(WRITE "readme_table.md" "${TABLE_CONTENT}")

# Get the number of lines in the table.
LIST(LENGTH README_TABLE_LINES README_TABLE_LENGTH)

# Get README.md content.
FILE(READ "${CMAKE_SOURCE_DIR}/README.md" README_FILE_CONTENT)

# Replace special list chars.
STRING(ASCII 2 SEMI_COLON)
STRING(REGEX REPLACE ";" "${SEMI_COLON}" README_FILE_CONTENT "${README_FILE_CONTENT}")

# Replace newlines.
STRING(REGEX REPLACE "[\r]?[\n]" ";" README_FILE_LINES "${README_FILE_CONTENT}")

# Get the number of lines in the file.
LIST(LENGTH README_FILE_LINES README_FILE_LENGTH)
IF(README_FILE_LENGTH LESS ${README_TABLE_LENGTH})
    MESSAGE(FATAL_ERROR "README.md is out of date: The file is too short.")
ENDIF()

# Check every line of the table is in the readme.
SET(README_OUT_OF_DATE FALSE)
SET(README_FILE_INDEX 11)
SET(README_TABLE_INDEX 0)
WHILE(README_TABLE_INDEX LESS ${README_TABLE_LENGTH})
    LIST(GET README_FILE_LINES ${README_FILE_INDEX} README_FILE_LINE)
    LIST(GET README_TABLE_LINES ${README_TABLE_INDEX} README_TABLE_LINE)
    IF(NOT README_FILE_LINE STREQUAL README_TABLE_LINE)
        MATH(EXPR README_FILE_INDEX "${README_FILE_INDEX}+1")
        SET(README_OUT_OF_DATE TRUE)
        BREAK()
    ENDIF()
    MATH(EXPR README_FILE_INDEX "${README_FILE_INDEX}+1")
    MATH(EXPR README_TABLE_INDEX "${README_TABLE_INDEX}+1")
ENDWHILE()

IF(README_OUT_OF_DATE)
    # Remove the old table from the readme.
    SET(README_FILE_INDEX 11)
    SET(README_TABLE_INDEX 0)
    WHILE(TRUE)
        LIST(GET README_FILE_LINES ${README_FILE_INDEX} README_FILE_LINE)
        IF(README_FILE_LINE STREQUAL "")
            BREAK()
        ENDIF()
        LIST(REMOVE_AT README_FILE_LINES ${README_FILE_INDEX})
    ENDWHILE()

    # Insert the new table.
    SET(README_FILE_INDEX 11)
    SET(README_TABLE_INDEX 0)
    WHILE(README_TABLE_INDEX LESS ${README_TABLE_LENGTH})
        LIST(GET README_TABLE_LINES ${README_TABLE_INDEX} README_TABLE_LINE)
        LIST(INSERT README_FILE_LINES ${README_FILE_INDEX} "${README_TABLE_LINE}")
        MATH(EXPR README_FILE_INDEX "${README_FILE_INDEX}+1")
        MATH(EXPR README_TABLE_INDEX "${README_TABLE_INDEX}+1")
    ENDWHILE()

    # Dump the updated readme to file.
    STRING(REGEX REPLACE ";" "\n" FILE_CONTENT "${README_FILE_LINES}")
    STRING(REGEX REPLACE "${SEMI_COLON}" ";" FILE_CONTENT "${FILE_CONTENT}")
    FILE(WRITE "${CMAKE_SOURCE_DIR}/README.md" "${FILE_CONTENT}")
ENDIF()

# Print an error message and fail if the readme was out of date.
IF(README_OUT_OF_DATE)
    MESSAGE(FATAL_ERROR "README.md is out of date.")
ENDIF()

MESSAGE(STATUS "Finished coverage of README.md file check.")
