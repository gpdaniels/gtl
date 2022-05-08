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

#include <debug/expect>

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

TEST(expect, evaluate, expect) {
    // Expecting.
    UNUSED(GTL_EXPECT_TRUE(true));
    UNUSED(GTL_EXPECT_FALSE(true));
    UNUSED(GTL_EXPECT_TRUE(false));
    UNUSED(GTL_EXPECT_FALSE(false));

    // Expecting in an if statement.
    if (GTL_EXPECT_TRUE(true)) {
        REQUIRE(true);
    }
    if (GTL_EXPECT_FALSE(true)) {
        REQUIRE(true);
    }
    if (GTL_EXPECT_TRUE(false)) {
        REQUIRE(false);
    }
    if (GTL_EXPECT_FALSE(false)) {
        REQUIRE(false);
    }

    // Expecting with a variable.
    {
        constexpr static const int variable1 = -1;
        double variable2 = 2.0;
        UNUSED(GTL_EXPECT_TRUE(variable1 < variable2));
        UNUSED(GTL_EXPECT_FALSE(variable1 < variable2));
        UNUSED(GTL_EXPECT_TRUE(variable1 < variable2));
        UNUSED(GTL_EXPECT_FALSE(variable1 < variable2));
        UNUSED(variable1);
        UNUSED(variable2);
    }

    // Expecting in an if statement, with {}.
    {
        if (true) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        }

        if (true) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
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
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        }
    }

    // Expecting in an if statement, without {}.
    {
        if (true)
            UNUSED(GTL_EXPECT_TRUE(true));
        if (true)
            UNUSED(GTL_EXPECT_FALSE(true));
        if (true)
            UNUSED(GTL_EXPECT_TRUE(false));
        if (true)
            UNUSED(GTL_EXPECT_FALSE(false));

        if (true)
            UNUSED(GTL_EXPECT_TRUE(true));
        else
            // This should never be reached.
            REQUIRE(false);

        if (true)
            UNUSED(GTL_EXPECT_FALSE(true));
        else
            // This should never be reached.
            REQUIRE(false);

        if (true)
            UNUSED(GTL_EXPECT_TRUE(false));
        else
            // This should never be reached.
            REQUIRE(false);

        if (true)
            UNUSED(GTL_EXPECT_FALSE(false));
        else
            // This should never be reached.
            REQUIRE(false);

        if (false)
            // This should never be reached.
            UNUSED(GTL_EXPECT_TRUE(true));
        else
            REQUIRE(true);

        if (false)
            // This should never be reached.
            UNUSED(GTL_EXPECT_FALSE(true));
        else
            REQUIRE(true);

        if (false)
            // This should never be reached.
            UNUSED(GTL_EXPECT_TRUE(false));
        else
            REQUIRE(true);

        if (false)
            // This should never be reached.
            UNUSED(GTL_EXPECT_FALSE(false));
        else
            REQUIRE(true);
    }

    // Expecting in a for loop, with {}.
    {
        for (bool loop = true; loop; loop = false) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        }

        for (bool loop = false; loop; loop = false) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        }
    }

    // Expecting in a for loop, without {}.
    {
        for (bool loop = true; loop; loop = false)
            UNUSED(GTL_EXPECT_TRUE(true));
        for (bool loop = true; loop; loop = false)
            UNUSED(GTL_EXPECT_FALSE(true));
        for (bool loop = true; loop; loop = false)
            UNUSED(GTL_EXPECT_TRUE(false));
        for (bool loop = true; loop; loop = false)
            UNUSED(GTL_EXPECT_FALSE(false));

        for (bool loop = false; loop; loop = false)
            UNUSED(GTL_EXPECT_TRUE(true));
        for (bool loop = false; loop; loop = false)
            UNUSED(GTL_EXPECT_FALSE(true));
        for (bool loop = false; loop; loop = false)
            UNUSED(GTL_EXPECT_TRUE(false));
        for (bool loop = false; loop; loop = false)
            UNUSED(GTL_EXPECT_FALSE(false));
    }

    // Expecting in a while loop, with {}.
    {
        bool loop = true;
        while (loop) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
            loop = false;
        }

        while (loop) {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        }
    }

    // Expecting in a while loop, without {}.
    {
        bool loop1 = true;
        while (loop1)
            UNUSED(GTL_EXPECT_TRUE(true)), loop1 = false;
        while (loop1)
            UNUSED(GTL_EXPECT_TRUE(true));

        bool loop2 = true;
        while (loop2)
            UNUSED(GTL_EXPECT_FALSE(true)), loop2 = false;
        while (loop2)
            UNUSED(GTL_EXPECT_FALSE(true));

        bool loop3 = true;
        while (loop3)
            UNUSED(GTL_EXPECT_TRUE(false)), loop3 = false;
        while (loop3)
            UNUSED(GTL_EXPECT_TRUE(false));

        bool loop4 = true;
        while (loop4)
            UNUSED(GTL_EXPECT_FALSE(false)), loop4 = false;
        while (loop4)
            UNUSED(GTL_EXPECT_FALSE(false));
    }

    // Expecting in a do while loop, with {}.
    {
        do {
            UNUSED(GTL_EXPECT_TRUE(true));
            UNUSED(GTL_EXPECT_FALSE(true));
            UNUSED(GTL_EXPECT_TRUE(false));
            UNUSED(GTL_EXPECT_FALSE(false));
        } while (false);
    }

    // Expecting in a do while loop, without {}.
    {
        do
            UNUSED(GTL_EXPECT_TRUE(true));
        while (false);
        do
            UNUSED(GTL_EXPECT_FALSE(true));
        while (false);
        do
            UNUSED(GTL_EXPECT_TRUE(false));
        while (false);
        do
            UNUSED(GTL_EXPECT_FALSE(false));
        while (false);
    }
}

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
