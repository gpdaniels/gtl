/*
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
        constexpr static auto handler =  [](int signal_number, siginfo_t* info, void* extra) {
            static_cast<void>(info);
            static_cast<void>(extra);
            if (signal_number == SIGTRAP) {
                PRINT("Breakpoint skipped.\n");
                caught = true;

                // Note: Arm cpus (unlike intel) do not increment the pc upon hitting the breakpoint instruction.
                #if defined(__APPLE__)
                    #if (defined(__arm__) || defined(_M_ARM))
                        #if defined(__thumb__)
                            reinterpret_cast<ucontext_t*>(extra)->uc_mcontext->__ss.__pc += 2;
                        #else
                            reinterpret_cast<ucontext_t*>(extra)->uc_mcontext->__ss.__pc += 4;
                        #endif
                    #elif (defined(__aarch64__))
                        reinterpret_cast<ucontext_t*>(extra)->uc_mcontext->__ss.__pc += 4;
                    #endif
                #else
                    #if (defined(__arm__) || defined(_M_ARM))
                        #if defined(__thumb__)
                            reinterpret_cast<ucontext_t*>(extra)->uc_mcontext.pc += 2;
                        #else
                            reinterpret_cast<ucontext_t*>(extra)->uc_mcontext.pc += 4;
                        #endif
                    #elif (defined(__aarch64__))
                        reinterpret_cast<ucontext_t*>(extra)->uc_mcontext.pc += 4;
                    #endif
                #endif
            }
        };

        struct sigaction action;
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = handler;
        REQUIRE(sigaction(SIGTRAP, &action, nullptr) != -1);
    #endif

    GTL_BREAKPOINT();

    REQUIRE(caught);
}

