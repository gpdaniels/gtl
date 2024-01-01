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

#include "optimise.tests.hpp"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <thread>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

namespace testbench {
    bool check_thread_id() {
        static std::thread::id thread_id = std::this_thread::get_id();
        return thread_id == std::thread::id();
    }

    void use_character(char character) {
        putchar(character);
    }
}

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   pragma GCC diagnostic pop
#endif
