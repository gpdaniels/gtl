/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <debug/breakpoint>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define STRICT

    #include <sdkddkver.h>

    #if defined(_AFXDLL)
        #include <afxwin.h>
    #else
        #include <Windows.h>
    #endif

    // AddVectoredExceptionHandler constants:
    // CALL_FIRST means call this exception handler first.
    // CALL_LAST means call this exception handler last.
    #define CALL_FIRST 1
    #define CALL_LAST 0
#else
    #include <csignal>

    #if !defined(SIGTRAP)
        #define SIGTRAP 5
    #endif
#endif

TEST(breakpoint, evaluate, breakpoint) {
    static bool caught = false;

    #if defined(_WIN32)
        constexpr static auto handler = [](EXCEPTION_POINTERS* exception) -> LONG {
            if (exception->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
                PRINT("Breakpoint at 0x%x skipped.\n", exception->ExceptionRecord->ExceptionAddress);

                PCONTEXT context = exception->ContextRecord;

                // The breakpoint instruction is 0xCC (int 3), just one byte in size.
                #ifdef _AMD64_
                     context->Rip++;
                #else
                     context->Eip++;
                #endif

                caught = true;

                return EXCEPTION_CONTINUE_EXECUTION;
            }
            return EXCEPTION_CONTINUE_SEARCH;
        };

        REQUIRE(AddVectoredExceptionHandler(CALL_FIRST, handler) != nullptr);
    #else
        constexpr static auto handler =  [](int signal_number) {
            if (signal_number == SIGTRAP) {
                PRINT("Breakpoint skipped.\n");
                caught = true;
            }
        };
        REQUIRE(std::signal(SIGTRAP, handler) != SIG_ERR);
    #endif

    GTL_BREAKPOINT();
    REQUIRE(caught);
}

