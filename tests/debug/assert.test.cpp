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
#include <require.tests.hpp>

#include <debug/assert>

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

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

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
