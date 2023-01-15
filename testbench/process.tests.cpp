/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include "process.tests.hpp"

#if defined(linux) || defined(__linux) || defined(__linux__)
#   include <linux/limits.h>
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <unistd.h>
#endif

#if defined(_WIN32)

#   if defined(_MSC_VER)
#       pragma warning(push, 0)
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
#       pragma warning(pop)
#   endif

#endif

#if defined(__APPLE__)
#   include <mach-o/dyld.h>
#   include <sys/syslimits.h>
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <unistd.h>
    extern char** environ;
#endif

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <cstdlib>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

namespace testbench {
    const char* get_executable()
    {
        #if defined(linux) || defined(__linux) || defined(__linux__)

            static char executable_path[PATH_MAX + 1] = {};
            ssize_t executable_path_size = ::readlink("/proc/self/exe", &executable_path[0], PATH_MAX);
            if (executable_path_size <= 0 || executable_path_size > PATH_MAX) {
                return nullptr;
            }
            return executable_path;

        #elif defined(_WIN32)

            static char executable_path[MAX_PATH + 1] = {};
            DWORD executable_path_size = GetModuleFileNameA(nullptr, &executable_path[0], MAX_PATH);
            if (executable_path_size <= 0 || executable_path_size > MAX_PATH) {
                return nullptr;
            }
            return executable_path;

        #elif defined(__APPLE__)

            static char executable_path[PATH_MAX + 1] = {};
            uint32_t executable_path_size = PATH_MAX;
            if (_NSGetExecutablePath(&executable_path[0], &executable_path_size) != 0) {
                return nullptr;
            }
            if (executable_path_size <= 0 || executable_path_size > PATH_MAX) {
                return nullptr;
            }
            return executable_path;

        #else

            return nullptr;

        #endif
    }

    int launch_process(const char* executable, const char* arguments[])
    {
        #if (defined(linux) || defined(__linux) || defined(__linux__)) || defined(__APPLE__)

            pid_t process_id = fork();

            // Ensure process creation was successful.
            if (process_id < 0) {
                return process_id;
            }

            if(process_id == 0) {

                // Execute self.
                if (execve(executable, const_cast<char* const*>(arguments), const_cast<char* const*>(environ)) < 0) {
                    std::abort();
                }

                // exec should never return.
                std::abort();
            }

            int status;
            if (waitpid(process_id, &status, 0) < 0) {
                return -1;
            }

            // Mask and shift the status code if it was a valid exit.
            if (WIFEXITED(status)) {
                status = WEXITSTATUS(status);
            }

            // Return the status code.
            return status;

        #elif defined(_WIN32)

            // Format the cmd line.
            char command[1024];
            snprintf(command, sizeof(command), "%s", executable);
            for (int argument_index = 0; arguments[argument_index]; ++argument_index) {
                strcat(command, " ");
                strcat(command, arguments[argument_index]);
            }

            PROCESS_INFORMATION process_information = {};
            STARTUPINFO startup_information = {};
            startup_information.cb = sizeof(startup_information);

            if (!CreateProcessA(nullptr, command, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_information, &process_information)) {
                return -1;
            }

            CloseHandle(process_information.hThread);

            DWORD wait_result = WaitForSingleObject(process_information.hProcess, INFINITE);
            if (wait_result != 0) {
                CloseHandle(process_information.hProcess);
                return -1;
            }

            // Try and get the return code.
            DWORD exit_code = 0;
            if (!GetExitCodeProcess(process_information.hProcess, &exit_code)) {
                CloseHandle(process_information.hProcess);
                return -1;
            }

            // Close the process.
            CloseHandle(process_information.hProcess);

            // Check the exit code.
            if (exit_code == STILL_ACTIVE) {
                return -1;
            }

        #   define STATUS_BAD_STACK 0xC0000028L

        #   define SIGINT           2
        #   define SIGILL           4
        #   define SIGABRT          6
        #   define SIGFPE           8
        #   define SIGSEGV          11

            switch (exit_code) {
                case STATUS_FLOAT_DENORMAL_OPERAND:
                case STATUS_FLOAT_DIVIDE_BY_ZERO:
                case STATUS_FLOAT_INEXACT_RESULT:
                case STATUS_FLOAT_INVALID_OPERATION:
                case STATUS_FLOAT_OVERFLOW:
                case STATUS_FLOAT_STACK_CHECK:
                case STATUS_FLOAT_UNDERFLOW:
                case STATUS_INTEGER_DIVIDE_BY_ZERO:
                case STATUS_INTEGER_OVERFLOW:
                    return SIGFPE;
                    break;
                case STATUS_ILLEGAL_INSTRUCTION:
                case STATUS_PRIVILEGED_INSTRUCTION:
                case STATUS_NONCONTINUABLE_EXCEPTION:
                    return SIGILL;
                    break;
                case STATUS_ACCESS_VIOLATION:
                case STATUS_DATATYPE_MISALIGNMENT:
                case STATUS_ARRAY_BOUNDS_EXCEEDED:
                case STATUS_GUARD_PAGE_VIOLATION:
                case STATUS_IN_PAGE_ERROR:
                case STATUS_NO_MEMORY:
                case STATUS_INVALID_DISPOSITION:
                case STATUS_BAD_STACK:
                case STATUS_STACK_OVERFLOW:
                    return SIGSEGV;
                    break;
                case STATUS_CONTROL_C_EXIT:
                    return SIGINT;
                    break;
            }
            if (exit_code & 0xC0000000) {
                return SIGABRT;
            }
            return exit_code;

        #else

            return -1;

        #endif
    }
}
