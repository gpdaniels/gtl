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

# Project name.
SET(PROJECT_NAME "gtl")
PROJECT(${PROJECT_NAME})

# Settings.
SET(BUILD_CHECKS ON CACHE BOOL "Enable automatic checking of source code for common mistakes using scripts in 'Check' directory.")
SET(BUILD_TESTING ON CACHE BOOL "Enable building individual unit test targets.")
SET(BUILD_MEMCHECK OFF CACHE BOOL "Enable running unit tests through valgrind memcheck.")
SET(BUILD_COVERAGE OFF CACHE BOOL "Enable generation of unit test code coverage data.")
SET(BUILD_SANITIZE_ADDRESS OFF CACHE BOOL "Enable building with clang sanitizer flags for address checking.")
SET(BUILD_SANITIZE_MEMORY OFF CACHE BOOL "Enable building with clang sanitizer flags memory checking.")
SET(BUILD_SANITIZE_THREAD OFF CACHE BOOL "Enable building with clang sanitizer flags thread checking.")
SET(BUILD_SANITIZE_UNDEFINED_BEHAVIOR OFF CACHE BOOL "Enable building with clang sanitizer flags for undefined behavior checking.")

SET(BUILD_TESTING_PARALLELISM 4 CACHE STRING "The number of tests to run in parallel.")
MARK_AS_ADVANCED(BUILD_TESTING_PARALLELISM)
SET(BUILD_TESTING_TIMEOUT 60 CACHE STRING "The allowed run time of a test executable.")
MARK_AS_ADVANCED(BUILD_TESTING_TIMEOUT)

SET(BUILD_MEMCHECK_PARALLELISM 4 CACHE STRING "The number of memcheck tests to run in parallel.")
MARK_AS_ADVANCED(BUILD_MEMCHECK_PARALLELISM)
SET(BUILD_MEMCHECK_TIMEOUT 60 CACHE STRING "The allowed run time of a memcheck test executable.")
MARK_AS_ADVANCED(BUILD_MEMCHECK_TIMEOUT)

SET(BUILD_TARGET_SEARCHPATH "*" CACHE STRING "A filter used to select which targets to include, default is everything.")

# Print status.
MESSAGE(STATUS "Configuring ${PROJECT_NAME}...")

################################################################################

# Emit compile commands in a "compile_commands.json" file.
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Enable testing if tests are enabled.
IF(BUILD_TESTING)
    ENABLE_TESTING()
ENDIF(BUILD_TESTING)

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
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
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
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
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
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:strict")
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
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
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

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/runtime/${CMAKE_BUILD_TYPE}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/archive/${CMAKE_BUILD_TYPE}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/library/${CMAKE_BUILD_TYPE}")

# Apply to all build types.
FOREACH(BUILD_TYPE ${CMAKE_CONFIGURATION_TYPES})
    STRING(TOUPPER "${BUILD_TYPE}" UPPER_BUILD_TYPE)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/runtime/${BUILD_TYPE}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/archive/${BUILD_TYPE}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPER_BUILD_TYPE}  "${CMAKE_BINARY_DIR}/library/${BUILD_TYPE}")
ENDFOREACH()

################################################################################

# Set clang sanitizer flags.
IF(BUILD_SANITIZE_ADDRESS OR BUILD_SANITIZE_MEMORY OR BUILD_SANITIZE_THREAD OR BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
        ENDIF()

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
        ENDIF()

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

        IF(BUILD_SANITIZE_ADDRESS)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_ADDRESS=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=address")
        ENDIF()

        IF(BUILD_SANITIZE_MEMORY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_MEMORY=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=memory")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=memory")
        ENDIF()

        IF(BUILD_SANITIZE_THREAD)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_THREAD=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=thread")
        ENDIF()

        IF(BUILD_SANITIZE_UNDEFINED_BEHAVIOR)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR=1")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
            #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined")
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
        
        #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fprofile-instr-generate")
        #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fcoverage-mapping")
    
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")

        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoverage-mapping")

        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fcoverage-mapping")

        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-instr-generate")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fcoverage-mapping")

        #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fprofile-instr-generate")
        #SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fcoverage-mapping")

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
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/source/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/tests/")

# Find all library source files in the test directory recursively.
FILE(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/${BUILD_TARGET_SEARCHPATH}/*")

# Find all test source files in the test directory recursively.
FILE(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/${BUILD_TARGET_SEARCHPATH}/*.test.cpp")

# Get the auxiliary testbench files.
FILE(GLOB_RECURSE TESTBENCH_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/testbench/*.tests.hpp")
FILE(GLOB_RECURSE TESTBENCH_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/testbench/*.tests.cpp")

################################################################################

# Enable threading.
SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

################################################################################

# Enable installing files.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    FILE(RELATIVE_PATH SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/" ${SOURCE_FILE})
    GET_FILENAME_COMPONENT(SOURCE_DIR "${SOURCE_DIR}" DIRECTORY)
    INSTALL(FILES ${SOURCE_FILE} DESTINATION "include/${PROJECT_NAME}/${SOURCE_DIR}")
ENDFOREACH()

################################################################################

# Get cmake system name in UPPER form.
STRING(TOUPPER ${CMAKE_SYSTEM_NAME} UPPER_CMAKE_SYSTEM_NAME)

# Create a list variable to hold detected #pragma linker flags.
SET(CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME} "")

# Parse header files for custom pragma.
FOREACH(SOURCE_FILE ${SOURCE_FILES})
    # Read the header file.
    FILE(READ ${SOURCE_FILE} TEST_CODE_FILE)

    # Replace block comments.
    STRING(ASCII 2 BLOCK_COMMENT_BEGIN)
    STRING(ASCII 3 BLOCK_COMMENT_CLOSE)
    STRING(REGEX REPLACE "[/][*]" "${BLOCK_COMMENT_BEGIN}" TEST_CODE_NOCOMMENTS "${TEST_CODE_FILE}")
    STRING(REGEX REPLACE "[*][/]" "${BLOCK_COMMENT_CLOSE}" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "${BLOCK_COMMENT_BEGIN}[^${BLOCK_COMMENT_CLOSE}]*${BLOCK_COMMENT_CLOSE}" "" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")
    STRING(REGEX REPLACE "\n[ \t]*//+[^\n]+" "\n" TEST_CODE_NOCOMMENTS "${TEST_CODE_NOCOMMENTS}")

    # Find all pragmas.
    STRING(REGEX MATCHALL "[#][ \t]*pragma[ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[\\(][ \t]*lib[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]" DETECTED_PRAGMAS "${TEST_CODE_NOCOMMENTS}")

    # Replace newlines.
    STRING(REGEX REPLACE "[\r]*[\n]+" ";" DETECTED_PRAGMAS "${DETECTED_PRAGMAS}")

    # Extract information from matches.
    FOREACH(DETECTED_PRAGMA ${DETECTED_PRAGMAS})
        IF("${DETECTED_PRAGMA}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*[#][ \t]*pragma[ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[\\(][ \t]*lib[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]")
            LIST(APPEND CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME} ${CMAKE_MATCH_1})
        ENDIF()
    ENDFOREACH()
ENDFOREACH()

IF(NOT "'${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}'" STREQUAL "''")
    MESSAGE(STATUS "Found additional linker flags: ${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}")
ENDIF()

################################################################################

# Count the number of tests found.
LIST(LENGTH TEST_FILES TEST_FILES_COUNT)
MESSAGE(STATUS "Found ${TEST_FILES_COUNT} tests.")

IF(BUILD_TESTING)
    # Build a testbench library.
    ADD_LIBRARY(testbench STATIC ${TESTBENCH_SOURCES} ${TESTBENCH_HEADERS})
    SET_TARGET_PROPERTIES(testbench PROPERTIES FOLDER CMakePredefinedTargets)

    # The Xcode generator automatically adds extra warning suppression flags, clear these.
    IF(CMAKE_GENERATOR STREQUAL Xcode)
        SET_TARGET_PROPERTIES(testbench PROPERTIES XCODE_ATTRIBUTE_WARNING_CFLAGS "")
    ENDIF()

    # Create an executable for each test file.
    FOREACH(TEST_FILE ${TEST_FILES})

        # Extract the name of the source and test files.
        FILE(RELATIVE_PATH TEST_FILE_NAME "${CMAKE_SOURCE_DIR}/tests/" ${TEST_FILE})
        STRING(REPLACE ".test.cpp" "" SOURCE_FILE_NAME ${TEST_FILE_NAME})
        SET(SOURCE_FILE "${CMAKE_SOURCE_DIR}/source/${SOURCE_FILE_NAME}")
        STRING(REPLACE "/" "_" TEST_TARGET_NAME ${SOURCE_FILE_NAME})

        FILE(RELATIVE_PATH TEST_FILE_NAME "${CMAKE_SOURCE_DIR}/tests/" ${TEST_FILE})

        # Print status.
        MESSAGE(STATUS "Preparing tests for '${TEST_TARGET_NAME}'...")

        # Compile the tests.
        ADD_EXECUTABLE(${TEST_TARGET_NAME} ${TEST_FILE} ${SOURCE_FILE})

        # Include the source and the testbench directory.
        TARGET_INCLUDE_DIRECTORIES(${TEST_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/source/)
        TARGET_INCLUDE_DIRECTORIES(${TEST_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/testbench/)

        # Set the folder for the test library equal to it's name.
        SET_TARGET_PROPERTIES(${TEST_TARGET_NAME} PROPERTIES FOLDER ${TEST_FILE_NAME})
        
        # The Xcode generator automatically adds extra warning suppression flags, clear these.
        IF(CMAKE_GENERATOR STREQUAL Xcode)
            SET_TARGET_PROPERTIES(${TEST_TARGET_NAME} PROPERTIES XCODE_ATTRIBUTE_WARNING_CFLAGS "")
        ENDIF()

        # Link to the testbench library.
        TARGET_LINK_LIBRARIES(${TEST_TARGET_NAME} PRIVATE testbench)

        # Link to the thread library.
        TARGET_LINK_LIBRARIES(${TEST_TARGET_NAME} PRIVATE Threads::Threads ${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}})

        # Add this test to the CTest list of tests to run.
        ADD_TEST(NAME ${TEST_TARGET_NAME} COMMAND $<TARGET_FILE:${TEST_TARGET_NAME}>)

        # Set a timeout to prevent long running tests.
        #SET_TESTS_PROPERTIES(${TEST_TARGET_NAME} PROPERTIES TIMEOUT ${BUILD_TESTING_TIMEOUT})

        # Set the working directory of the test.
        SET_TESTS_PROPERTIES(${TEST_TARGET_NAME} PROPERTIES REQUIRED_FILES $<TARGET_FILE:${TEST_TARGET_NAME}> WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

        # Set the code coverage output path.
        SET_TESTS_PROPERTIES(${TEST_TARGET_NAME} PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/coverage/data/${TEST_TARGET_NAME}-%p.profraw")

        # Create a list of coverage objects.
        LIST(APPEND BUILD_COVERAGE_OBJECTS $<TARGET_FILE:${TEST_TARGET_NAME}>)

    ENDFOREACH()
ELSE()
    MESSAGE(STATUS "Skipping building tests.")
ENDIF()

###############################################################################

# Add a cmake target.
ADD_CUSTOM_TARGET(gtl_cmake
    COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running CMake..."
    COMMAND "${CMAKE_COMMAND}" "-H${CMAKE_SOURCE_DIR}" "-B${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
SET_TARGET_PROPERTIES(gtl_cmake PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_cmake PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_cmake PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

SET(ADDITIONAL_BUILD_OPTIONS "")
IF(WIN32)
    SET(ADDITIONAL_BUILD_OPTIONS "--" "-nologo" "-verbosity:quiet" "-maxcpucount")
ELSE()
    SET(ADDITIONAL_BUILD_OPTIONS "--" "-j4")
ENDIF()

# Add a build target.
ADD_CUSTOM_TARGET(gtl_build
    COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Building..."
    COMMAND "${CMAKE_COMMAND}" "--build" "." "--config" "$<CONFIG>" ${ADDITIONAL_BUILD_OPTIONS}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
SET_TARGET_PROPERTIES(gtl_build PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_build PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_build PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

IF(BUILD_TESTING AND BUILD_COVERAGE)
    # Add a cleanup target.
    ADD_CUSTOM_TARGET(gtl_cleanup
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running cleanup..."
        COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${CMAKE_BINARY_DIR}/Coverage"
        DEPENDS gtl_build
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ELSE()
    ADD_CUSTOM_TARGET(gtl_cleanup
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Cleanup not required."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()
SET_TARGET_PROPERTIES(gtl_cleanup PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_cleanup PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_cleanup PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

IF(BUILD_TESTING)
    # Add a test target.
    ADD_CUSTOM_TARGET(gtl_test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running tests..."
        COMMAND "${CMAKE_CTEST_COMMAND}" "-C" "$<CONFIG>" "--repeat" "after-timeout:3" "--output-on-failure" "--parallel" "${BUILD_TESTING_PARALLELISM}" "--timeout" "${BUILD_TESTING_TIMEOUT}"
        DEPENDS gtl_build gtl_cleanup
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ELSE()
    ADD_CUSTOM_TARGET(gtl_test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Testing not enabled."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()
SET_TARGET_PROPERTIES(gtl_test PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_test PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

IF(BUILD_TESTING AND BUILD_MEMCHECK)
    # Add memcheck tests if valgrind is installed
    FIND_PROGRAM(VALGRIND_EXECUTABLE NAMES valgrind)
    IF(VALGRIND_EXECUTABLE)
        # Set valgrind options.
        SET(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_SOURCE_DIR}/memcheck.supression")

        ADD_CUSTOM_TARGET(gtl_memcheck
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running tests under memcheck..."
            COMMAND "${CMAKE_CTEST_COMMAND}" "-C" "$<CONFIG>" "--output-on-failure" "--parallel" "${BUILD_MEMCHECK_PARALLELISM}" "--timeout" "${BUILD_MEMCHECK_TIMEOUT}" "--force-new-ctest-process" "--test-action" "memcheck"
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
SET_TARGET_PROPERTIES(gtl_memcheck PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_memcheck PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_memcheck PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

IF(BUILD_TESTING AND BUILD_COVERAGE)
    FIND_PROGRAM(LLVM_PROFDATA_EXECUTABLE NAMES llvm-profdata)
    FIND_PROGRAM(LLVM_COV_EXECUTABLE NAMES llvm-cov)
    IF(LLVM_PROFDATA_EXECUTABLE AND LLVM_COV_EXECUTABLE)
        # Prepare the coverage objects for the command line.
        STRING(REPLACE ";" ";-object;" BUILD_COVERAGE_OBJECTS "${BUILD_COVERAGE_OBJECTS}")

        ADD_CUSTOM_TARGET(gtl_coverage
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Generating code coverage report..."
            COMMAND "${LLVM_PROFDATA_EXECUTABLE}" "merge" "-output=./coverage/data/coverage.profdata" "./coverage/data/*.profraw"
            COMMAND "${LLVM_COV_EXECUTABLE}" "report" "-format=text" "-instr-profile=./coverage/data/coverage.profdata" ${BUILD_COVERAGE_OBJECTS} ${SOURCE_FILES} ${TEST_FILES}
            COMMAND "${LLVM_COV_EXECUTABLE}" "show" "-format=html" "-instr-profile=./coverage/data/coverage.profdata" "-output-dir=./coverage/" ${BUILD_COVERAGE_OBJECTS} ${SOURCE_FILES} ${TEST_FILES}
            COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Detailed code coverage results are in '${CMAKE_BINARY_DIR}/coverage/index.html'."
            DEPENDS gtl_build gtl_test
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
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
SET_TARGET_PROPERTIES(gtl_coverage PROPERTIES FOLDER CMakePredefinedTargets)
SET_TARGET_PROPERTIES(gtl_coverage PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(gtl_coverage PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

# Override the test command such that tests are built and ctest provides output on error.
IF(BUILD_TESTING)
    SET(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
    CMAKE_POLICY(PUSH)
    CMAKE_POLICY(SET CMP0037 OLD)
    ADD_CUSTOM_TARGET(test
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running test pipeline..."
        DEPENDS gtl_build gtl_test gtl_memcheck gtl_coverage
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    SET_TARGET_PROPERTIES(test PROPERTIES FOLDER CMakePredefinedTargets)
    SET_TARGET_PROPERTIES(test PROPERTIES EXCLUDE_FROM_ALL TRUE)
    SET_TARGET_PROPERTIES(test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    CMAKE_POLICY(POP)
    UNSET(CMAKE_WARN_DEPRECATED CACHE)
ENDIF()

###############################################################################

IF(BUILD_CHECKS)
    # Find all check scripts in the checks directory.
    FILE(GLOB CHECK_FILES "${CMAKE_SOURCE_DIR}/checks/*.[Cc][Mm][Aa][Kk][Ee]")
    
    # Add a checking target to attach all the post builds to and add it to the ALL target.
    ADD_CUSTOM_TARGET(gtl_check ALL
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Running checks..."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    
    # Include all found scripts.
    FOREACH(CHECK_FILE ${CHECK_FILES})
        ADD_CUSTOM_COMMAND(
            TARGET gtl_check
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" "-DSOURCE_DIR=${CMAKE_SOURCE_DIR}" "-DBINARY_DIR=${CMAKE_BINARY_DIR}" "-P" "${CHECK_FILE}"
            COMMENT "Running ${CHECK_FILE}..."
        )
    ENDFOREACH()
ELSE()
    # Add a dummy checking target so that the target exists.
    ADD_CUSTOM_TARGET(gtl_check
        COMMAND "${CMAKE_COMMAND}" "-E" "echo" "No checks found."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()

###############################################################################

# Print status.
MESSAGE(STATUS "Finished configuring ${PROJECT_NAME}.")
