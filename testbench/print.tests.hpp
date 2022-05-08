/*
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
*/

#pragma once
#ifndef GTL_PRINT_TESTS_HPP
#define GTL_PRINT_TESTS_HPP

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

#elif (defined(__clang__)) || ((defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER))))

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
    enum class output_stream {
        output,
        error
    };

    void disable_output_buffering(output_stream stream);

    PRINT_FORMAT_PRINT_DECORATION(1) int print(output_stream stream, PRINT_FORMAT_PRINT_ARGUMENT(const char* format), ...);
}

/// @brief  Print the output format.
#define PRINT(...) (testbench::print(testbench::output_stream::error, __VA_ARGS__))

#endif // GTL_PRINT_TESTS_HPP
