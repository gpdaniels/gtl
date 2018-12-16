# Define the minimum version of CMake that is required.
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
CMAKE_POLICY(VERSION 3.5.1)

# Project name.
SET(PROJECT_NAME "gtl")
PROJECT(${PROJECT_NAME})

# Print status.
MESSAGE(STATUS "Configuring ${PROJECT_NAME}...")

# Require c11
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

# Require c++14
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags.
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
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
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
ELSE("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    MESSAGE(WARNING "Unsupported compiler.")
ENDIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

# Add the source and test folder to the include path.
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Source/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Tests/")

# Find all library source files in the test directory recursively.
FILE(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/Source/*")

# Find all test source files in the test directory recursively.
FILE(GLOB TEST_SOURCE_FILES "${PROJECT_SOURCE_DIR}/Tests/*.test.cpp")

# Get the main testbench files.
SET(TESTS_MAIN_HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Testbench/main.tests.hpp")
SET(TESTS_MAIN_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Testbench/main.tests.cpp")

# Get the auxiliary testbench files.
FILE(GLOB TESTS_HEADER_FILES "${PROJECT_SOURCE_DIR}/Testbench/*.tests.hpp")
FILE(GLOB TESTS_SOURCE_FILES "${PROJECT_SOURCE_DIR}/Testbench/*.tests.cpp")

# Remove the main test files from the auxiliary.
LIST(REMOVE_ITEM TESTS_HEADER_FILES ${TESTS_MAIN_HEADER_FILE})
LIST(REMOVE_ITEM TESTS_SOURCE_FILES ${TESTS_MAIN_SOURCE_FILE})

# Enable threading.
SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

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
    STRING(REGEX REPLACE "\n[ \t]*//+[^\n]+" "\n" ${TEST_CODE_NOCOMMENTS} "${TEST_CODE_NOCOMMENTS}")

    # Find all pragmas.
    STRING(REGEX MATCHALL "[#][ \t]*pragma[ \t]*comment[ \t]*[\\(][ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]" DETECTED_PRAGMAS "${TEST_CODE_NOCOMMENTS}")

    # Replace newlines.
    STRING(REGEX REPLACE "[\r]*[\n]+" ";" DETECTED_PRAGMAS "${DETECTED_PRAGMAS}")

    # Extract information from matches.
    FOREACH(DETECTED_PRAGMA ${DETECTED_PRAGMAS})
        IF("${DETECTED_PRAGMA}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*[#][ \t]*pragma[ \t]*comment[ \t]*[\\(][ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]")
            LIST(APPEND CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME} ${CMAKE_MATCH_1})
        ENDIF("${DETECTED_PRAGMA}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*[#][ \t]*pragma[ \t]*comment[ \t]*[\\(][ \t]*CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}[ \t]*[,][ \t]*[\"]((-l|-framework )[0-9a-zA-Z_]*)[\"][ \t]*[\\)]")
    ENDFOREACH(DETECTED_PRAGMA ${DETECTED_PRAGMAS})

ENDFOREACH(HEADER_FILE ${HEADER_FILES})

IF(NOT "'${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}'" STREQUAL "''")
    MESSAGE(STATUS "Found additional linker flags: ${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}")
ENDIF(NOT "'${CMAKE_LINKER_FLAG_${UPPER_CMAKE_SYSTEM_NAME}}'" STREQUAL "''")

# Enable testing.
ENABLE_TESTING()

# Override the test command such that tests are built and ctest provides output on error.
CMAKE_POLICY(PUSH)
CMAKE_POLICY(SET CMP0037 OLD)
ADD_CUSTOM_TARGET(test
    COMMAND ${CMAKE_COMMAND} --build . --config $<CONFIG>
    COMMAND echo ""
    COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --parallel
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
CMAKE_POLICY(POP)

# Enable grouping by folder in an IDE.
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

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
    STRING(REGEX REPLACE "\n[ \t]*//+[^\n]+" "\n" ${TEST_CODE_NOCOMMENTS} "${TEST_CODE_NOCOMMENTS}")

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
        ENDIF("${DETECTED_TEST}" MATCHES "^[ \t]*[^/]*[^/]*[ \t]*TEST[ \t]*[\\(][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[,][ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*[\\)]")

    ENDFOREACH(DETECTED_TEST ${DETECTED_TESTS})

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

    # Print status.
    MESSAGE(STATUS "Preparing tests for file '${TEST_FILE_NAME}'...")

    # For each test found in the test file, create a test executable.
    WHILE(SUB_TEST_INDEX LESS ${SUB_TEST_COUNT})

        # Extract the test name and ground from the lists.
        LIST(GET SUB_TEST_GROUPS ${SUB_TEST_INDEX} SUB_TEST_GROUP)
        LIST(GET SUB_TEST_NAMES  ${SUB_TEST_INDEX} SUB_TEST_NAME)

        # Print status.
        MESSAGE(STATUS "Preparing test ${SUB_TEST_INDEX} [Group: '${SUB_TEST_GROUP}', Test: '${SUB_TEST_NAME}']")

        # Create a name for this test from the parts.
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
        SET_TESTS_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES TIMEOUT 1)

        # Set the working directory of the test.
        SET_TESTS_PROPERTIES(${CURRENT_TEST_NAME} PROPERTIES REQUIRED_FILES $<TARGET_FILE:${CURRENT_TEST_NAME}> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

        # Increment the loop index.
        MATH(EXPR SUB_TEST_INDEX "${SUB_TEST_INDEX} + 1")

    ENDWHILE(SUB_TEST_INDEX LESS ${SUB_TEST_COUNT})

    MESSAGE(STATUS " ")

ENDFOREACH(TEST_FILE ${TEST_SOURCE_FILES})

# Print status.
MESSAGE(STATUS "Finished configuring ${PROJECT_NAME}.")
