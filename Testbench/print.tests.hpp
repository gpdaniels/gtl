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
#ifndef PRINT_TESTS_HPP
#define PRINT_TESTS_HPP

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#if defined(_MSC_VER)

#   if defined(_MSC_VER)
#      pragma warning(push, 0)
#   endif

    // Include the SAL annotations header to validate format strings.
#   include <sal.h>

#   if defined(_MSC_VER)
#      pragma warning(pop)
#   endif

    // MSVC needs to wrap the format string argument.
#   if (_MSC_VER > 1400)
#       define PRINT_FORMAT_PRINT_ARGUMENT(ARGUMENT)    _Printf_format_string_ ARGUMENT
#       define PRINT_FORMAT_SCAN_ARGUMENT(ARGUMENT)     _Scanf_format_string_  ARGUMENT
#   else
#       define PRINT_FORMAT_PRINT_ARGUMENT(ARGUMENT)    __format_string ARGUMENT
#       define PRINT_FORMAT_SCAN_ARGUMENT(ARGUMENT)     __format_string ARGUMENT
#   endif

    // MSVC doens't use any function decoration for checking format string arguments.
#   define PRINT_FORMAT_PRINT_DECORATION(ARGUMENT_INDEX)
#   define PRINT_FORMAT_SCAN_DECORATION(ARGUMENT_INDEX)

#elif (defined(__clang__)) || ((defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER)))

//  Clang and GCC don't need to wrap the format string argument.
#define PRINT_FORMAT_PRINT_ARGUMENT(ARGUMENT)           ARGUMENT
#define PRINT_FORMAT_SCAN_ARGUMENT(ARGUMENT)            ARGUMENT

//  Clang and GCC use an attribute for checking format string arguments.
#define PRINT_FORMAT_PRINT_DECORATION(ARGUMENT_INDEX)   __attribute__((__format__(__printf__, ARGUMENT_INDEX + 1, ARGUMENT_INDEX + 2)))
#define PRINT_FORMAT_SCAN_DECORATION(ARGUMENT_INDEX)    __attribute__((__format__(__scanf__,  ARGUMENT_INDEX + 1, ARGUMENT_INDEX + 2)))

#else

#   define PRINT_FORMAT_PRINT_ARGUMENT(ARGUMENT)        ARGUMENT
#   define PRINT_FORMAT_SCAN_ARGUMENT(ARGUMENT)         ARGUMENT

#   define PRINT_FORMAT_PRINT_DECORATION(ARGUMENT_INDEX)
#   define PRINT_FORMAT_SCAN_DECORATION(ARGUMENT_INDEX)

#endif

namespace testbench {
    PRINT_FORMAT_PRINT_DECORATION(1) int print(FILE* stream, PRINT_FORMAT_PRINT_ARGUMENT(const char* format), ...);
}

/// @brief  Print the output format.
#define PRINT(...) (testbench::print(stderr, __VA_ARGS__))

#endif // PRINT_TESTS_HPP
