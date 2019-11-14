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
#include <benchmark.tests.hpp>
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <type/type_name>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(type_name, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(sizeof(gtl::type_name<type>) >= 1, "sizeof(gtl::type_name<type>) = %ld, expected >= %lld", sizeof(gtl::type_name<type>), 1ull);
        }
    );

    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(std::is_pod<gtl::type_name<type> >::value == true, "Expected std::is_pod to be true.");
        }
    );

    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(std::is_trivial<gtl::type_name<type> >::value == true, "Expected std::is_trivial to be true.");
        }
    );

    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(std::is_trivially_copyable<gtl::type_name<type> >::value == true, "Expected std::is_trivially_copyable to be true.");
        }
    );

    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(std::is_standard_layout<gtl::type_name<type> >::value == true, "Expected std::is_standard_layout to be true.");
        }
    );
}

TEST(type_name, constructor, empty) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_name<type> type_name;
            testbench::do_not_optimise_away(type_name);
        }
    );
}

TEST(type_name, function, name) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_name<type> type_name;
            REQUIRE(testbench::is_string_same(type_name.name(), testbench::test_data<type>::name) == true, "gtl::type_name<type>::name() = '%s', expected '%s'", type_name.name(), testbench::test_data<type>::name);
        }
    );
}
