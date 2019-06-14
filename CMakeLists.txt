#
# The MIT License
# Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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

# Project name.
SET(PROJECT_NAME "gtl")
PROJECT(${PROJECT_NAME})

# Settings.
SET(BUILD_TESTING OFF CACHE BOOL "Enable building individual unit test targets.")
SET(BUILD_MEMCHECK OFF CACHE BOOL "Enable running unit tests through valgrind memcheck.")
SET(BUILD_COVERAGE OFF CACHE BOOL "Enable generation of unit test code coverage data.")
SET(BUILD_SANITIZE_ADDRESS OFF CACHE BOOL "Enable building with clang sanitizer flags for address checking.")
SET(BUILD_SANITIZE_MEMORY OFF CACHE BOOL "Enable building with clang sanitizer flags memory checking.")
SET(BUILD_SANITIZE_THREAD OFF CACHE BOOL "Enable building with clang sanitizer flags thread checking.")
SET(BUILD_SANITIZE_UNDEFINED_BEHAVIOR OFF CACHE BOOL "Enable building with clang sanitizer flags for undefined behavior checking.")

# Print status.
MESSAGE(STATUS "Configuring ${PROJECT_NAME}...")

################################################################################

# Require c11.
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

# Require c++17.
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Position independent code.
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Visual studio warning level.
SET(CMAKE_CXX_WARNING_LEVEL 4)
SET(CMAKE_C_WARNING_LEVEL 4)

# Enable grouping by folder in an IDE.
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################

# Check for the build type only.
IF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
ENDIF()

# Check for the configuration type.
IF(NOT CMAKE_CONFIGURATION_TYPES)
    # If there was no configuration type then set it to the build type.
    SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
ENDIF()

################################################################################

# Set compiler build flags.
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-newline-eof")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-newline-eof")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP8")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_HAS_EXCEPTIONS=0")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ITERATOR_DEBUG_LEVEL=0")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES=1")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX=1")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_ATOMIC_ALIGNMENT_FIX=1")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_NONSTDC_NO_WARNINGS=1")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS=1")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported compiler.")
ENDIF()

################################################################################

# Set compiler linker flags.
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    # None
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # None
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # None
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported compiler.")
ENDIF()

################################################################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/Runtime/${CMAKE_BUILD_TYPE}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/Archive/${CMAKE_BUILD_TYPE}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/Library/${CMAKE_BUILD_TYPE}")

# Apply to all build types.
FOREACH(BUILD_TYPE ${CMAKE_CONFIGURATION_TYPES})
    STRING(TOUPPER "${BUILD_TYPE}" UPPER_BUILD_TYPE)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/Runtime/${BUILD_TYPE}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/Archive/${BUILD_TYPE}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/Library/${BUILD_TYPE}")
ENDFOREACH()

################################################################################

# Set clang sanitizer flags.
IF(BUILD_SANITIZE_ADDRESS OR BUILD_SANITIZE_MEMORY OR BUILD_SANITIZE_THREAD OR BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
        ENDIF()

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
        ENDIF()

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
        ENDIF()

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        MESSAGE(FATAL_ERROR "Unsupported compiler.")
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported compiler.")
    ENDIF()
ENDIF()


################################################################################

# Set code coverage flags.
IF(BUILD_COVERAGE)
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoverage-mapping")
        
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fcoverage-mapping")
        
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fcoverage-mapping")
        
        SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fcoverage-mapping")
    
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoverage-mapping")

        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fcoverage-mapping")

        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fcoverage-mapping")

        SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fcoverage-mapping")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        MESSAGE(FATAL_ERROR "Unsupported compiler.")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        MESSAGE(FATAL_ERROR "Unsupported compiler.")
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported compiler.")
    ENDIF()
ENDIF()

################################################################################

# Add the source and test folder to the include path.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/Source/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/Tests/")

# Find all library source files in the test directory recursively.
FILE(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*")

# Find all test source files in the test directory recursively.
FILE(GLOB_RECURSE TEST_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Tests/*.test.cpp")

# Get the main testbench files.
SET(TESTS_MAIN_HEADER_FILE "${CMAKE_SOURCE_DIR}/Testbench/main.tests.hpp")
SET(TESTS_MAIN_SOURCE_FILE "${CMAKE_SOURCE_DIR}/Testbench/main.tests.cpp")

# Get the auxiliary testbench files.
FILE(GLOB_RECURSE TESTS_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Testbench/*.tests.hpp")
FILE(GLOB_RECURSE TESTS_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Testbench/*.tests.cpp")

# Remove the main test files from the auxiliary.
LIST(REMOVE_ITEM TESTS_HEADER_FILES ${TESTS_MAIN_HEADER_FILE})
LIST(REMOVE_ITEM TESTS_SOURCE_FILES ${TESTS_MAIN_SOURCE_FILE})

################################################################################

# Enable threading.
SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

################################################################################

# Get cmake system name in UPPER form.
STRING(TOUPPER ${CMAKE_SYSTEM_NAME} UPPER_CMAKE_SYSTEM_NAME)

# Create a list variable to hold detected #pragma linker flags.
SET(CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME} "")

# Parse header files for custom pragma.
FOREACH(HEADER_FILE ${HEADER_FILES})
    # Read the header file.
    FILE(READ ${HEADER_FILE} TEST_CODE_FILE)

    # Replace block comments.
    STRING(ASCII 2 BLOCK_COMMENT_BEGIN)
    STRING(ASCII 3 BLOCK_COMMENT_CLOSE)
    STRING(REGEX REPLACE "[/][*]" "${BLOCK_COMMENT_BEGIN}" TEST_CODE_NOCOMMENTS "${TEST_CODE_FILE}")
    STRING(REGEX REPLACE "[*][/]" "${BLOCK_COMMENT_CLOSE}" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "${BLOCK_COMMENT_BEGIN}[^${BLOCK_COMMENT_CLOSE}]*${BLOCK_COMMENT_CLOSE}" "" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "\n[ \t]*//+[^\n]+" "\n" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")

    # Find all pragmas.
    STRING(REGEX MATCHALL "[#][ \t]*pragma[ \t]*comment[ \t]*[\\(][ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]" DETECTED_PRAGMAS "${TEST_CODE_NOCOMMENTS}")

    # Replace newlines.
    STRING(REGEX REPLACE "[\r]*[\n]+" ";" DETECTED_PRAGMAS "${DETECTED_PRAGMAS}")

    # Extract information from matches.
    FOREACH(DETECTED_PRAGMA ${DETECTED_PRAGMAS})
        IF("${DETECTED_PRAGMA}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*[#][ \t]*pragma[ \t]*comment[ \t]*[\\(][ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]")
            LIST(APPEND CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME} ${CMAKE_MATCH_1})
        ENDIF()
    ENDFOREACH()

ENDFOREACH()

IF(NOT "'${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}'" STREQUAL "''")
    MESSAGE(STATUS "Found additional linker flags: ${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}")
ENDIF()

################################################################################

IF(BUILD_TESTING)
    ENABLE_TESTING()
ENDIF(BUILD_TESTING)

# Add a build target.
ADD_CUSTOM_TARGET(gtl_build
    COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Building..."
    COMMAND "${CMAKE_COMMAND}" "--build" "." "--config" "$<CONFIG>"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

IF(BUILD_TESTING AND BUILD_COVERAGE)
    # Add a cleanup target.
    ADD_CUSTOM_TARGET(gtl_cleanup
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running cleanup..."
        COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${CMAKE_BINARY_DIR}/Coverage/Data"
        DEPENDS gtl_build
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ELSE()
    ADD_CUSTOM_TARGET(gtl_cleanup
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Cleanup not required."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()

IF(BUILD_TESTING)
    # Add a test target.
    ADD_CUSTOM_TARGET(gtl_test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running tests..."
        COMMAND "${CMAKE_CTEST_COMMAND}" "-C" "$<CONFIG>" "--output-on-failure" "--parallel" "4" "--timeout" "10"
        DEPENDS gtl_build gtl_cleanup
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ELSE()
    ADD_CUSTOM_TARGET(gtl_test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Testing not enabled."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()

IF(BUILD_TESTING AND BUILD_MEMCHECK)
    # Add memcheck tests if valgrind is installed
    FIND_PROGRAM(VALGRIND_EXECUTABLE NAMES valgrind)
    IF(VALGRIND_EXECUTABLE)
        # Set valgrind options.
        SET(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_SOURCE_DIR}/memcheck.supression")
        
        ADD_CUSTOM_TARGET(gtl_memcheck
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running tests under memcheck..."
            COMMAND "${CMAKE_CTEST_COMMAND}" "-C" "$<CONFIG>" "--output-on-failure" "--parallel" "4" "--timeout" "30" "--force-new-ctest-process" "--test-action" "memcheck"
            COMMAND "cat" "./Testing/Temporary/MemoryChecker.*.log"
            DEPENDS gtl_build gtl_test
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )

        # Create a memcheck dart configuration.
        FILE(WRITE  "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "# Auto generated by CMake.\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "Site: ${CMAKE_SYSTEM_NAME}\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "BuildName: ${CMAKE_CXX_COMPILER}\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "MemoryCheckCommand: ${VALGRIND_EXECUTABLE}\n")
        FILE(APPEND "${CMAKE_BINARY_DIR}/DartConfiguration.tcl" "MemoryCheckSuppressionFile: ${CMAKE_SOURCE_DIR}/memcheck.supression\n")
    ELSE()
        ADD_CUSTOM_TARGET(gtl_memcheck
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Memcheck not available."
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    ENDIF()
ELSE()
    ADD_CUSTOM_TARGET(gtl_memcheck
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Memcheck not enabled."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()

IF(BUILD_TESTING AND BUILD_COVERAGE)
    FIND_PROGRAM(LLVM_PROFDATA_EXECUTABLE NAMES llvm-profdata)
    FIND_PROGRAM(LLVM_COV_EXECUTABLE NAMES llvm-cov)
    IF(LLVM_PROFDATA_EXECUTABLE AND LLVM_COV_EXECUTABLE)
        ADD_CUSTOM_TARGET(gtl_coverage
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Generating code coverage report..."
            COMMAND "${LLVM_PROFDATA_EXECUTABLE}" "merge" "-output=./Coverage/Coverage.profdata" "./Coverage/Data/*.profraw"
            COMMAND "${CMAKE_COMMAND}" "-P" "./Coverage/GenerateReport.cmake"
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Code coverage results are in '${CMAKE_BINARY_DIR}/Coverage/'."
            DEPENDS gtl_build gtl_test
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )

        # Create a coverage report script.
        SET(COVERAGE_SCRIPT "# Auto generated by CMake.\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}FIND_PROGRAM(LLVM_COV_EXECUTABLE NAMES llvm-cov)\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}IF(NOT LLVM_COV_EXECUTABLE)\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}    MESSAGE(FATAL_ERROR)\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}ENDIF()\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}\n")
    ELSE()
        ADD_CUSTOM_TARGET(gtl_coverage
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Code coverage not available."
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    ENDIF()
ELSE()
    ADD_CUSTOM_TARGET(gtl_coverage
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Code coverage not enabled."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()

# Override the test command such that tests are built and ctest provides output on error.
IF(BUILD_TESTING)
    CMAKE_POLICY(PUSH)
    CMAKE_POLICY(SET CMP0037 OLD)
    ADD_CUSTOM_TARGET(test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running test pipeline..."
        DEPENDS gtl_build gtl_test gtl_memcheck gtl_coverage
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    SET_TARGET_PROPERTIES(test PROPERTIES FOLDER CMakePredefinedTargets)
    CMAKE_POLICY(POP)
ENDIF()

################################################################################

# Count the number of tests found.
LIST(LENGTH TEST_SOURCE_FILES TEST_SOURCE_FILES_COUNT)
STRING(LENGTH "${TEST_SOURCE_FILES_COUNT}" TEST_SOURCE_FILES_COUNT_LENGTH)
MESSAGE(STATUS "Found ${TEST_SOURCE_FILES_COUNT} tests.")

# Create an executable for each test in each test file.
FOREACH(TEST_FILE ${TEST_SOURCE_FILES})

    # Extract the name of the test file.
    GET_FILENAME_COMPONENT(TEST_FILE_NAME ${TEST_FILE} NAME)
    STRING(REPLACE ".test.cpp" "" TEST_FILE_NAME ${TEST_FILE_NAME})
    STRING(REPLACE "/" "_" TEST_FILE_NAME ${TEST_FILE_NAME})

    # Lists for the test names and their classes
    SET(SUB_TEST_GROUPS "")
    SET(SUB_TEST_NAMES "")

    # Read the test file.
    FILE(READ ${TEST_FILE} TEST_CODE_FILE)

    # Replace block comments.
    STRING(ASCII 2 BLOCK_COMMENT_BEGIN)
    STRING(ASCII 3 BLOCK_COMMENT_CLOSE)
    STRING(REGEX REPLACE "[/][*]" "${BLOCK_COMMENT_BEGIN}" TEST_CODE_NOCOMMENTS "${TEST_CODE_FILE}")
    STRING(REGEX REPLACE "[*][/]" "${BLOCK_COMMENT_CLOSE}" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "${BLOCK_COMMENT_BEGIN}[^${BLOCK_COMMENT_CLOSE}]*${BLOCK_COMMENT_CLOSE}" "" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "\n[ \t]*//+[^\n]+" "\n" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")

    # Find all tests.
    STRING(REGEX MATCHALL "TEST[ \t]*[\\(][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[,][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[\\)]" DETECTED_TESTS "${TEST_CODE_NOCOMMENTS}")

    # Replace newlines.
    STRING(REGEX REPLACE "[\r]*[\n]+" ";" DETECTED_TESTS "${DETECTED_TESTS}")

    # Extract information from matches.
    FOREACH(DETECTED_TEST ${DETECTED_TESTS})

        # C++ only allows letters, digits and the underscore characters in variable names. => [a-zA-Z_][0-9a-zA-Z_]*
        IF("${DETECTED_TEST}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*TEST[ \t]*[\\(][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[,][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[\\)]")
            LIST(APPEND SUB_TEST_GROUPS ${CMAKE_MATCH_1})
            LIST(APPEND SUB_TEST_NAMES ${CMAKE_MATCH_2})
        ENDIF()

    ENDFOREACH()

    # Count the number of tests found.
    LIST(LENGTH SUB_TEST_GROUPS SUB_TEST_COUNT)

    # Set the starting index to zero.
    SET(SUB_TEST_INDEX 0)

    # Compile the tests in this file into an object library.
    ADD_LIBRARY(${TEST_FILE_NAME} OBJECT ${TEST_FILE} ${TESTS_MAIN_HEADER_FILE} ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES} ${HEADER_FILES})

    # Include the source and the testbench directory.
    TARGET_INCLUDE_DIRECTORIES(${TEST_FILE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
    TARGET_INCLUDE_DIRECTORIES(${TEST_FILE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Testbench/)

    # Set the folder for the test libraray equal to it's name.
    SET_TARGET_PROPERTIES(${TEST_FILE_NAME} PROPERTIES FOLDER ${TEST_FILE_NAME})

    # Create a coverage report script.
    IF(BUILD_TESTING AND BUILD_COVERAGE)
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}SET(TARGET_OBJECTS \"$<TARGET_OBJECTS:${TEST_FILE_NAME}>\")\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}LIST(GET TARGET_OBJECTS 0 TARGET_OBJECT)\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}EXECUTE_PROCESS(\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}    COMMAND \"\${LLVM_COV_EXECUTABLE}\" \"show\" \"-format=html\" \"-instr-profile=./Coverage/Coverage.profdata\" \"\${TARGET_OBJECT}\" \"${CMAKE_SOURCE_DIR}/Source\"\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}    OUTPUT_FILE \"./Coverage/${TEST_FILE_NAME}.html\"\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}    WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\"\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT})\n")
        SET(COVERAGE_SCRIPT "${COVERAGE_SCRIPT}\n")
    ENDIF()

    # Print status.
    MESSAGE(STATUS "Preparing tests for file '${TEST_FILE_NAME}'...")

    # For each test found in the test file, create a test executable.
    WHILE(SUB_TEST_INDEX LESS ${SUB_TEST_COUNT})

        # Extract the test name and ground from the lists.
        LIST(GET SUB_TEST_GROUPS ${SUB_TEST_INDEX} SUB_TEST_GROUP)
        LIST(GET SUB_TEST_NAMES  ${SUB_TEST_INDEX} SUB_TEST_NAME)

        # Pad test index with zeros to make it three chars long.
        STRING(LENGTH "${SUB_TEST_INDEX}" SUB_TEST_INDEX_LENGTH)
        SET(SUB_TEST_INDEX_PADDED "${SUB_TEST_INDEX}")
        MATH(EXPR PADDING_LENGTH "${TEST_SOURCE_FILES_COUNT_LENGTH} - ${SUB_TEST_INDEX_LENGTH}")
        WHILE(PADDING_LENGTH GREATER 0)
            MATH(EXPR PADDING_LENGTH "${PADDING_LENGTH} - 1")
            SET(SUB_TEST_INDEX_PADDED "0${SUB_TEST_INDEX_PADDED}")
        ENDWHILE(PADDING_LENGTH GREATER 0)

        # Print status.
        MESSAGE(STATUS "Preparing test ${SUB_TEST_INDEX_PADDED} [Group: '${SUB_TEST_GROUP}', Test: '${SUB_TEST_NAME}']")

        # Create a name for this test from the parts.
        #SET(CURRENT_TEST_NAME ${TEST_FILE_NAME}_${SUB_TEST_INDEX_PADDED}_${SUB_TEST_GROUP}_${SUB_TEST_NAME})
        SET(CURRENT_TEST_NAME ${TEST_FILE_NAME}_${SUB_TEST_GROUP}_${SUB_TEST_NAME})
        
        # Create the test executable.
        ADD_EXECUTABLE(${CURRENT_TEST_NAME} ${TESTS_MAIN_SOURCE_FILE} ${TESTS_MAIN_HEADER_FILE} ${TESTS_HEADER_FILES} ${HEADER_FILES})

        # Set the folder for each test executable to the same as the test library folder.
        SET_TARGET_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES FOLDER ${TEST_FILE_NAME})

        # Only enable the one test in the test file using a compile definition.
        TARGET_COMPILE_DEFINITIONS(${CURRENT_TEST_NAME} PRIVATE SUB_TEST_GROUP=${SUB_TEST_GROUP} SUB_TEST_NAME=${SUB_TEST_NAME})

        # Link to the test library.
        TARGET_LINK_LIBRARIES(${CURRENT_TEST_NAME} PRIVATE $<TARGET_OBJECTS:${TEST_FILE_NAME}>)

        # Link to the thread library.
        TARGET_LINK_LIBRARIES(${CURRENT_TEST_NAME} PRIVATE Threads::Threads ${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}})

        # Set the dependency of the test on the test library.
        ADD_DEPENDENCIES(${CURRENT_TEST_NAME} ${TEST_FILE_NAME})

        # Add this test to the CTest list of tests to run.
        ADD_TEST(NAME ${CURRENT_TEST_NAME} COMMAND $<TARGET_FILE:${CURRENT_TEST_NAME}>)

        # Set a timeout to prevent long running tests.
        #SET_TESTS_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES TIMEOUT 10)

        # Set the working directory of the test.
        SET_TESTS_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES REQUIRED_FILES $<TARGET_FILE:${CURRENT_TEST_NAME}> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        
        # Set the code coverage output path.
        SET_TESTS_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/Coverage/Data/%m.profraw")
        
        # Increment the loop index.
        MATH(EXPR SUB_TEST_INDEX "${SUB_TEST_INDEX} + 1")

    ENDWHILE()

    MESSAGE(STATUS " ")

ENDFOREACH()

IF(BUILD_TESTING AND BUILD_COVERAGE)
    FILE(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/Coverage/GenerateReport.cmake" CONTENT "${COVERAGE_SCRIPT}")
ENDIF()

# Print status.
MESSAGE(STATUS "Finished configuring ${PROJECT_NAME}.")
