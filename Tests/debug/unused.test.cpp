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

#include <debug/unused>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
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

