/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#pragma once
#ifndef MACRO_TESTS_HPP
#define MACRO_TESTS_HPP

#include <cstdio>

/// @brief  A helper macro to prevent compiler warnings about unused variables.
#define UNUSED(VARIABLE) (static_cast<void>(VARIABLE))

// Define a print macro that wraps printf, this is more complex than it needs to be as we can't use ##__VA_ARGS__ if we want to remain standards complient.
// The common non-standards complient method that requires a gnu extension is: #define PRINT(FORMAT, ...) printf(FORMAT "\n", ##__VA_ARGS__), in c++20 __VA_OPT__ is coming which will fix this issue.
// As a work around count the number of variadic arguments and either call a macro with just the format string, if there is none, or with the arguments.
// This method is limited in that the maximum number of arguments has to be known ahead of time.

/// @brief  Used to append a value onto a function name to allow selecting a function depending on the number of arguments provided.
#define SELECT_FUNCTION(FUNCTION, \
    ARG_48, ARG_47, ARG_46, ARG_45, ARG_44, ARG_43, ARG_42, ARG_41, ARG_40, ARG_39, ARG_38, ARG_37, ARG_36, ARG_35, ARG_34, ARG_33, \
    ARG_32, ARG_31, ARG_30, ARG_29, ARG_28, ARG_27, ARG_26, ARG_25, ARG_24, ARG_23, ARG_22, ARG_21, ARG_20, ARG_19, ARG_18, ARG_17, \
    ARG_16, ARG_15, ARG_14, ARG_13, ARG_12, ARG_11, ARG_10, ARG_09, ARG_08, ARG_07, ARG_06, ARG_05, ARG_04, ARG_03, ARG_02, ARG_01, SUFFIX, ...) FUNCTION ## _ ## SUFFIX

/// @brief  Print the output format, with no arguments.
#define PRINT_1(FORMAT)      (fprintf(stderr, FORMAT "\n"))

/// @brief  Print the output format, with the provided arguments.
#define PRINT_N(FORMAT, ...) (fprintf(stderr, FORMAT "\n", __VA_ARGS__))

/// @brief  Print the output by counting the number of arguments to select the correct PRINT_* macro.
#define PRINT(...) UNUSED(SELECT_FUNCTION(PRINT, __VA_ARGS__, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, 1, 0)(__VA_ARGS__))

/// @brief  Variable to hold the number of "REQUIRE" calls.
extern unsigned long long REQUIRE_COUNT;

/// @brief  Variable to hold the number of "REQUIRE" failures.
extern unsigned long long REQUIRE_FAILURE_COUNT;

/// @brief  Test an assertion and print output if the test fails.
#define REQUIRE_1(TEST) UNUSED(((++REQUIRE_COUNT), (TEST)) || ( \
    PRINT("REQUIRE failure:"), \
    PRINT("  Failure #: '%lld'", ++REQUIRE_FAILURE_COUNT), \
    PRINT("  File:      '%s'", __FILE__), \
    PRINT("  Line:      '%d'", __LINE__), \
    PRINT("  Function:  '%s'", __func__), \
    PRINT("  Assertion: '%s'", #TEST), 0))

#define REQUIRE_2(TEST, FORMAT) UNUSED((TEST) || ( \
    REQUIRE_1(TEST), \
    PRINT("  Output:    '" FORMAT "'"), 0))

#define REQUIRE_N(TEST, FORMAT, ...) UNUSED((TEST) || ( \
    REQUIRE_1(TEST), \
    PRINT("  Output:    '" FORMAT "'", __VA_ARGS__), 0))

/// @brief  Print the output by counting the number of arguments to select the correct PRINT_* macro.
#define REQUIRE(...) UNUSED(SELECT_FUNCTION(REQUIRE, __VA_ARGS__, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, \
    N, N, N, N, N, N, N, N, N, N, N, N, N, N, 2, 1, 0)(__VA_ARGS__))

/// @brief  Static assert macro to assert requirments at build time.
#define STATIC_REQUIRE(TEST) (static_assert((TEST), "STATIC_REQUIRE failure: " #TEST))

#endif // MACRO_TESTS_HPP
