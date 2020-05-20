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
#include <require.tests.hpp>

#include <debug/assume>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
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

        for (bool loop = false; loop; loop = false) {
            GTL_ASSUME(false);
        }
    }
    
    // Assuming in a for loop, without {}.
    {
        for (bool loop = true; loop; loop = false)
            GTL_ASSUME(true);

        for (bool loop = false; loop; loop = false)
            GTL_ASSUME(false);
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
