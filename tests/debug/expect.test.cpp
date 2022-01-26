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
