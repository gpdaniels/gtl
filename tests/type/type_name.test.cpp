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
#include <testbench/optimise.tests.hpp>
#include <testbench/comparison.tests.hpp>
#include <testbench/data.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

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
