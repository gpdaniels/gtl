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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <debug/unused>

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

TEST(unused, evaluate, unused) {

    // Unused with a variable.
    int variable;
    GTL_UNUSED(variable);

    // Unused with a value.
    GTL_UNUSED(true);
    GTL_UNUSED(1);
    GTL_UNUSED(static_cast<unsigned long long int>(0xFF00FF00FF00FFull));

    // Unused with a comparison.
    {
        constexpr static const int variable1 = -1;
        volatile double variable2 = 2.0;
        GTL_UNUSED(variable1 < variable2);
    }

    // Unused in an if statement, with {}.
    {
        if (true) {
            GTL_UNUSED(true);
        }

        if (true) {
            GTL_UNUSED(true);
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
            GTL_UNUSED(true);
        }
    }

    // Unused in an if statement, without {}.
    {
        if (true)
            GTL_UNUSED(true);

        if (true)
            GTL_UNUSED(true);
        else
            // This should never be reached.
            REQUIRE(false);

        if (false)
            // This should never be reached.
            GTL_UNUSED(false);
        else
            REQUIRE(true);
    }

    // Unused in a for loop, with {}.
    {
        for (bool loop = true; loop; loop = false) {
            GTL_UNUSED(true);
        }

        for (bool loop = false; loop; loop = false) {
            GTL_UNUSED(false);
        }
    }

    // Unused in a for loop, without {}.
    {
        for (bool loop = true; loop; loop = false)
            GTL_UNUSED(true);

        for (bool loop = false; loop; loop = false)
            GTL_UNUSED(false);
    }

    // Unused in a while loop, with {}.
    {
        bool loop = true;
        while (loop) {
            GTL_UNUSED(true);
            loop = false;
        }

        while (loop) {
            GTL_UNUSED(false);
        }
    }

    // Unused in a while loop, without {}.
    {
        bool loop = true;
        while (loop)
            GTL_UNUSED(true), loop = false;

        while (loop)
            GTL_UNUSED(false);
    }

    // Unused in a do while loop, with {}.
    {
        do {
            GTL_UNUSED(true);
        } while (false);
    }

    // Unused in a do while loop, without {}.
    {
        do
            GTL_UNUSED(true);
        while (false);
    }
}

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
