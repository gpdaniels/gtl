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

#include "print.tests.hpp"
#include "unused.tests.hpp"

#if defined(_WIN32)
#   if defined(_MSC_VER)
#      pragma warning(push, 0)
#   endif
#   define WIN32_LEAN_AND_MEAN
#   define VC_EXTRALEAN
#   define STRICT
#   include <sdkddkver.h>
#   if defined(_AFXDLL)
#       include <afxwin.h>
#   else
#       include <Windows.h>
#   endif
#   if defined(_MSC_VER)
#      pragma warning(pop)
#   endif
#endif

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdarg>
#include <cstdio>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

namespace testbench {
    void disable_output_buffering(output_stream stream) {
        FILE* handle = nullptr;
        switch (stream) {
        case output_stream::output:
            handle = stdout;
            break;
        case output_stream::error:
            handle = stderr;
            break;
        }
        setvbuf(handle, nullptr, _IONBF, 0);
    }

    PRINT_FORMAT_PRINT_DECORATION(1) int print(output_stream stream, PRINT_FORMAT_PRINT_ARGUMENT(const char* format), ...) {
        FILE* handle = nullptr;
        switch (stream) {
        case output_stream::output:
            handle = stdout;
            break;
        case output_stream::error:
            handle = stderr;
            break;
        }

        #if defined(_WIN32)

            char outputString[1024] = {};

            va_list formatArguments;
            va_start(formatArguments, format);
            int outputCountDebug = _vsnprintf(outputString, sizeof(outputString), format, formatArguments);
            va_end(formatArguments);

            ::OutputDebugStringA(outputString);

            int outputCountStdOut = std::fprintf(handle, "%s", outputString);

            std::fflush(handle);

            UNUSED(outputCountStdOut);
            return outputCountDebug;

        #else

            va_list formatArguments;
            va_start(formatArguments, format);
            int outputCount = std::vfprintf(handle, format, formatArguments);
            va_end(formatArguments);

            std::fflush(handle);

            return outputCount;

        #endif
    }
}
