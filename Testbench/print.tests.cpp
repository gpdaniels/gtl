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

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

PRINT_FORMAT_PRINT_DECORATION(1) int print(FILE* stream, PRINT_FORMAT_PRINT_ARGUMENT(const char* format), ...) {
#if defined(_WIN32)

    char outputString[1024] = {};

    va_list formatArguments;
    va_start(formatArguments, format);
    int outputCountDebug = _vsnprintf(outputString, sizeof(outputString), format, formatArguments);
    va_end(formatArguments);

    ::OutputDebugStringA(outputString);

    int outputCountStdOut = std::fprintf(stream, "%s", outputString);

    std::fflush(stream);

    UNUSED(outputCountStdOut);
    return outputCountDebug;

#else

    va_list formatArguments;
    va_start(formatArguments, format);
    int outputCount = std::vfprintf(stream, format, formatArguments);
    va_end(formatArguments);

    std::fflush(stream);

    return outputCount;

#endif
}