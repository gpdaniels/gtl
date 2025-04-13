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

#include "abort.tests.hpp"

extern "C" int raise(int signal_number);

namespace testbench {
    [[noreturn]] void abort() {
        // Try and raise a sigabort.
        constexpr static const int sigabort = 6;
        raise(sigabort);

// Silence the static analyser as the below is an intentional segfault.
#ifndef __clang_analyzer__
        // Force a segfault just in case.
        *static_cast<volatile char*>(nullptr) = static_cast<char (*)()>(nullptr)();
#endif

        // This function can never return.
        for (;;)
            ;
    }
}
