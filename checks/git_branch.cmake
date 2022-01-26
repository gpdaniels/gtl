#
# The MIT License
# Copyright (c) 2021 Geoffrey Daniels. http://gpdaniels.com/
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
IF(NOT "${COMMIT_BRANCH}" MATCHES "^(master)|(dev)|(feature/.+)|(bug-fix/.+)$")
    MESSAGE(FATAL_ERROR "The current branch '${COMMIT_BRANCH}' is not valid.")
ENDIF()
        
MESSAGE(STATUS "Finished git branch check.")

