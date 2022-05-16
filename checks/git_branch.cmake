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

MESSAGE(STATUS "Checking for git branch...")

# Get parameters passed from the main CMakeLists.txt.
SET(CMAKE_SOURCE_DIR ${SOURCE_DIR})

# Get the git executable.
FIND_PROGRAM(GIT_EXECUTABLE NAMES git DOC "Git command line client")
IF(NOT GIT_EXECUTABLE)
    MESSAGE(FATAL_ERROR "Git executable not found.")
ENDIF()

# Use git to get the branch name.
EXECUTE_PROCESS(
    COMMAND             "${GIT_EXECUTABLE}" "rev-parse" "--abbrev-ref" "HEAD"
    WORKING_DIRECTORY   "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE     RETURN_CODE
    OUTPUT_VARIABLE     COMMIT_BRANCH
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Check for errors running git.
IF(RETURN_CODE)
    MESSAGE(FATAL_ERROR "Failed to get the git branch.")
ENDIF(RETURN_CODE)

# Check the git branch matches one of the valid patterns.
IF(NOT "${COMMIT_BRANCH}" MATCHES "^master$|^dev$|^feature/.+$|^bug-fix/.+$|^HEAD$")
    MESSAGE(FATAL_ERROR "The current branch '${COMMIT_BRANCH}' is not valid.")
ENDIF()
        
MESSAGE(STATUS "Finished git branch check.")

