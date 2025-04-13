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

#pragma once
#ifndef GTL_OPTIMISE_TESTS_HPP
#define GTL_OPTIMISE_TESTS_HPP

#include "abort.tests.hpp"

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

namespace testbench {
    bool check_thread_id();
    void use_character(char character);

    template <typename type>
    void do_not_optimise_away(type&& value) {
        // To prevent value being optimised away it needs to be used somwhere.
        // When using the value it must not impact the benchmark being performed.
        // Therefore, use the value inside a never executed if block.

        // Compilers are smart enough that using an if(false) block is not enough.
        // An if block is required that will never execute and complex enough that the compiler cannot remove it.
        // By checking against the current thread id the compiler cannot know ahead of time if the test will pass.
        if (check_thread_id()) {
            // Once inside the if block we must now "use" the value.
            for (unsigned long long int index = 0; index < sizeof(type); ++index) {
                use_character(reinterpret_cast<const char*>(&value)[index]);
            }
            // To sanity check that this block of code is never reached, abort.
            testbench::abort();
        }
    }
}

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#pragma GCC diagnostic pop
#endif

#endif // GTL_OPTIMISE_TESTS_HPP
