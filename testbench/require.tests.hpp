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
#ifndef GTL_REQUIRE_TESTS_HPP
#define GTL_REQUIRE_TESTS_HPP

#include "ignored.tests.hpp"
#include "print.tests.hpp"
#include "unused.tests.hpp"

namespace testbench {
    /// @brief  Variable to hold the number of "REQUIRE" calls.
    extern unsigned long long REQUIRE_COUNT;

    /// @brief  Variable to hold the number of "REQUIRE" failures.
    extern unsigned long long REQUIRE_FAILURE_COUNT;
}

// Attempt to use a more detailed function macro if possible.
#if defined(__clang__)
#   define REQUIRE_FUNCTION __PRETTY_FUNCTION__
#elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   define REQUIRE_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#   define REQUIRE_FUNCTION __FUNCSIG__
#else
#   define REQUIRE_FUNCTION __func__
#endif

// Macro magic from assert.
#define REQUIRE_PRIMITIVE_CAT(FIRST_ARGUMENT, ...) FIRST_ARGUMENT ## __VA_ARGS__
#define REQUIRE_PRIMITIVE_TO_STRING(ARGUMENT) #ARGUMENT
#define REQUIRE_TO_STRING(ARGUMENT) REQUIRE_PRIMITIVE_TO_STRING(ARGUMENT)
#define REQUIRE_COMPLIMENT(BOOLEAN_TEST) REQUIRE_PRIMITIVE_CAT(REQUIRE_COMPLIMENT_, BOOLEAN_TEST)
#define REQUIRE_COMPLIMENT_0 1
#define REQUIRE_COMPLIMENT_1 0
#define REQUIRE_ARGUMENT_1(ARGUMENT_1, ...) ARGUMENT_1
#define REQUIRE_ARGUMENT_2(ARGUMENT_1, ARGUMENT_2, ...) ARGUMENT_2
#define REQUIRE_ARGUMENTS_2_N(ARGUMENT_1, ...) __VA_ARGS__
#define REQUIRE_EXPAND(...) __VA_ARGS__
#define REQUIRE_EMPTY()
#define REQUIRE_DEFER(ARGUMENT) ARGUMENT REQUIRE_EMPTY()
#define REQUIRE_CHECK(...) REQUIRE_EXPAND(REQUIRE_ARGUMENT_2(__VA_ARGS__, 0,))
#define REQUIRE_PROBE(IGNORED) IGNORED, 1,
#define REQUIRE_NOT(TEST) REQUIRE_CHECK(REQUIRE_EXPAND(REQUIRE_PRIMITIVE_CAT(REQUIRE_NOT_, TEST)))
#define REQUIRE_NOT_0 REQUIRE_PROBE(~)
#define REQUIRE_BOOLEAN(TEST) REQUIRE_COMPLIMENT(REQUIRE_NOT(TEST))
#define REQUIRE_IF(BOOLEAN_TEST) REQUIRE_PRIMITIVE_CAT(REQUIRE_IF_, BOOLEAN_TEST)
#define REQUIRE_IF_0(THEN, ...) __VA_ARGS__
#define REQUIRE_IF_1(THEN, ...) THEN
#define REQUIRE_END_FLAG() 0
#define REQUIRE_HAS_TWO_ARGUMENTS(...) REQUIRE_BOOLEAN(REQUIRE_EXPAND(REQUIRE_DEFER(REQUIRE_ARGUMENT_1)(REQUIRE_END_FLAG REQUIRE_ARGUMENTS_2_N(__VA_ARGS__,),)()))

/// @brief  Main require macro.
#define REQUIRE(...)                                                                                    \
    REQUIRE_IF(REQUIRE_HAS_TWO_ARGUMENTS(__VA_ARGS__))(                                                 \
        UNUSED(((++testbench::REQUIRE_COUNT) && (REQUIRE_EXPAND(REQUIRE_ARGUMENT_1(__VA_ARGS__)))) || ( \
            PRINT("Requirement failure:  #%lld\n",  ++testbench::REQUIRE_FAILURE_COUNT),                \
            PRINT("  Requirement:        %s\n", REQUIRE_TO_STRING(REQUIRE_ARGUMENT_1(__VA_ARGS__,))),   \
            PRINT("  File:               %s\n", __FILE__),                                              \
            PRINT("  Line:               %d\n", __LINE__),                                              \
            PRINT("  Function:           %s\n", REQUIRE_FUNCTION),                                      \
            PRINT("  Additional Output:  "),                                                            \
            PRINT(REQUIRE_EXPAND(REQUIRE_ARGUMENTS_2_N(__VA_ARGS__))),                                  \
            PRINT("\n"),                                                                                \
            0)                                                                                          \
        )                                                                                               \
    /*REQUIRE_ELSE*/,                                                                                   \
        UNUSED(((++testbench::REQUIRE_COUNT) && (__VA_ARGS__)) || (                                     \
            PRINT("Requirement failure:  #%lld\n",  ++testbench::REQUIRE_FAILURE_COUNT),                \
            PRINT("  Requirement:        %s\n", REQUIRE_TO_STRING(REQUIRE_ARGUMENT_1(__VA_ARGS__,))),   \
            PRINT("  File:               %s\n", __FILE__),                                              \
            PRINT("  Line:               %d\n", __LINE__),                                              \
            PRINT("  Function:           %s\n", REQUIRE_FUNCTION),                                      \
            0)                                                                                          \
        )                                                                                               \
    )

/// @brief  Silent version of the require macro.
#define REQUIRE_SILENT(...)                                                                             \
    REQUIRE_IF(REQUIRE_HAS_TWO_ARGUMENTS(__VA_ARGS__))(                                                 \
        UNUSED(((++testbench::REQUIRE_COUNT) && (REQUIRE_EXPAND(REQUIRE_ARGUMENT_1(__VA_ARGS__)))) || ( \
            (++testbench::REQUIRE_FAILURE_COUNT),                                                       \
            0)                                                                                          \
        )                                                                                               \
    /*REQUIRE_ELSE*/,                                                                                   \
        UNUSED(((++testbench::REQUIRE_COUNT) && (__VA_ARGS__)) || (                                     \
            (++testbench::REQUIRE_FAILURE_COUNT),                                                       \
            0)                                                                                          \
        )                                                                                               \
    )

#endif // GTL_REQUIRE_TESTS_HPP
