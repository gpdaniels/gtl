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

#include <testbench/main.tests.hpp>
#include <testbench/require.tests.hpp>

#include <debug/assume>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4702)
#endif

TEST(assume, evaluate, assume) {
    // Assuming without a message.
    GTL_ASSUME(true);

    // Assuming with a variable.
    {
        constexpr static const int variable1 = -1;
        double variable2 = 2.0;
        GTL_ASSUME(variable1 < variable2);
        UNUSED(variable1);
        UNUSED(variable2);
    }

    // Assuming in an if statement, with {}.
    {
        if (true) {
            GTL_ASSUME(true);
        }
        
        if (true) {
            GTL_ASSUME(true);
        }
        else {
            // This should never be reached.
            REQUIRE(false);
        }

        if (false) {
            // This should never be reached.
            REQUIRE(false);
        }
        else {
            GTL_ASSUME(true);
        }
    }

    // Assuming in an if statement, without {}.
    {
        if (true)
            GTL_ASSUME(true);

        if (true)
            GTL_ASSUME(true);
        else
            // This should never be reached.
            REQUIRE(false);

        if (false)
            // This should never be reached.
            GTL_ASSUME(false);
        else
            REQUIRE(true);
    }
    
    // Assuming in a for loop, with {}.
    {
        for (bool loop = true; loop; loop = false) {
            GTL_ASSUME(true);
        }

        #if defined(__clang__)
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wunreachable-code-loop-increment"
        #endif

        for (bool loop = false; loop; loop = false) {
            GTL_ASSUME(false);
        }

        #if defined(__clang__)
            #pragma clang diagnostic pop
        #endif
    }
    
    // Assuming in a for loop, without {}.
    {
        for (bool loop = true; loop; loop = false)
            GTL_ASSUME(true);

        #if defined(__clang__)
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wunreachable-code-loop-increment"
        #endif

        for (bool loop = false; loop; loop = false)
            GTL_ASSUME(false);

        #if defined(__clang__)
            #pragma clang diagnostic pop
        #endif
    }
    
    // Assuming in a while loop, with {}.
    {
        bool loop = true;
        while (loop) {
            GTL_ASSUME(true);
            loop = false;
        }

        while (loop) {
            GTL_ASSUME(false);
        }
    }
    
    // Assuming in a while loop, without {}.
    {
        bool loop = true;
        while (loop)
            UNUSED(GTL_ASSUME(true)), loop = false;

        while (loop)
            GTL_ASSUME(false);
    }

    // Assuming in a do while loop, with {}.
    {
        do {
            GTL_ASSUME(true);
        } while (false);
    }
    
    // Assuming in a do while loop, without {}.
    {
        do
            GTL_ASSUME(true);
        while (false);
    }
}

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
