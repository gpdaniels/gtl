/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
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

#include <debug/assert>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(assert, traits, standard) {
}

TEST(assert, evaluate, assert) {
    // Asserting without a message.
    GTL_ASSERT(true);
    
    // Asserting with a message.
    GTL_ASSERT(true, "Message.");
    
    // Asserting with a format string and parameter.
    GTL_ASSERT(true, "%s", "Parameter.");
    
    // Asserting with a variable.
    {
        constexpr static const int variable1 = -1;
        volatile double variable2 = 2.0;
        GTL_ASSERT(variable1 < variable2);
    }
    
    // Asserting in an if statement, with {}.
    {
        if (true) {
            GTL_ASSERT(true);
        }
        
        if (true) {
            GTL_ASSERT(true);
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
            GTL_ASSERT(true);
        }
    }

    // Asserting in an if statement, without {}.
    {
        if (true)
            GTL_ASSERT(true);

        if (true)
            GTL_ASSERT(true);
        else
            // This should never be reached.
            REQUIRE(false);

        if (false)
            // This should never be reached.
            GTL_ASSERT(false);
        else
            REQUIRE(true);
    }
    
    // Asserting in a for loop, with {}.
    {
        for (bool loop = true; loop; loop = false) {
            GTL_ASSERT(true);
        }

        for (bool loop = false; loop; loop = false) {
            GTL_ASSERT(false);
        }
    }
    
    // Asserting in a for loop, without {}.
    {
        for (bool loop = true; loop; loop = false)
            GTL_ASSERT(true);

        for (bool loop = false; loop; loop = false)
            GTL_ASSERT(false);
    }
    
    // Asserting in a while loop, with {}.
    {
        bool loop = true;
        while (loop) {
            GTL_ASSERT(true);
            loop = false;
        }

        while (loop) {
            GTL_ASSERT(false);
        }
    }
    
    // Asserting in a while loop, without {}.
    {
        bool loop = true;
        while (loop)
            GTL_ASSERT(true), loop = false;

        while (loop)
            GTL_ASSERT(false);
    }

    // Asserting in a do while loop, with {}.
    {
        do {
            GTL_ASSERT(true);
        } while (false);
    }
    
    // Asserting in a do while loop, without {}.
    {
        do
            GTL_ASSERT(true);
        while (false);
    }
}

namespace gtl_assert_test {
    static bool triggered = false;
    void abort();
    void abort() {
        triggered = true;
    }
} 

// The assert uses std::abort to quit, this redefines the namespace of the abort function to the above.
#define std gtl_assert_test

TEST(assert, trigger, assert) {
    gtl_assert_test::triggered = false;
    GTL_ASSERT(false);
    REQUIRE(gtl_assert_test::triggered);
    
    gtl_assert_test::triggered = false;
    GTL_ASSERT(false, "\n");
    REQUIRE(gtl_assert_test::triggered);
    
    gtl_assert_test::triggered = false;
    GTL_ASSERT(false, "Message\n");
    REQUIRE(gtl_assert_test::triggered);
    
    gtl_assert_test::triggered = false;
    GTL_ASSERT(false, "%s\n", "Parameter.");
    REQUIRE(gtl_assert_test::triggered);
}

#undef std
