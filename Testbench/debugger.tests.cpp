/*
The MIT License
Copyright (c) 2021 Geoffrey Daniels. http://gpdaniels.com/
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

#include "debugger.tests.hpp"

#if defined(linux) || defined(__linux) || defined(__linux__)
    #include <sys/stat.h>
    #include <string.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <ctype.h>
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
    #include <stdbool.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/sysctl.h>
#endif

namespace testbench {
    bool is_debugger_attached() {
        #if defined(linux) || defined(__linux) || defined(__linux__)
            // Read the status file.
            const int status_handle = ::open("/proc/self/status", O_RDONLY);
            // If we failed to open the file, assume false.
            if (status_handle == -1) {
                return false;
            }
            char status_buffer[4096];
            const ssize_t status_size = ::read(status_handle, status_buffer, sizeof(status_buffer) - 1);
            ::close(status_handle);
            // If we failed to read any data, assume false.
            if (status_size <= 0) {
                return false;
            }
            status_buffer[status_size] = '\0';
            // Search for the tracer string.
            constexpr static const char tracer_match_string[11] = "TracerPid:";
            const char* tracer_string = ::strstr(status_buffer, tracer_match_string);
            // If we failed to find the string, assume false.
            if (!tracer_string) {
                return false;
            }
            for (const char* character = tracer_string + sizeof(tracer_match_string) - 1; character <= status_buffer + status_size; ++character) {
                // Skip over space characters.
                if (::isspace(*character)) {
                    continue;
                }
                // The status of debugger attachment is determined by the next digit not being a zero.
                return ((::isdigit(*character) != 0) && (*character != '0'));
            }
            // If the status string does not contain enough data, assume false.
            return false;
        #elif defined(_WIN32)
            return static_cast<bool>(IsDebuggerPresent());
        #elif defined(__APPLE__)
            int mib[4] = {
                CTL_KERN,
                KERN_PROC,
                KERN_PROC_PID,
                getpid()
            };
            kinfo_proc info = {};
            size_t info_size = sizeof(info);
            int return_code = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &info_size, NULL, 0);
            // Check if the syscall failed.
            if (return_code == 0) {
                return false;
            }
            return ((info.kp_proc.p_flag & P_TRACED) != 0);
        #else
            return false;
        #endif
    }
}
