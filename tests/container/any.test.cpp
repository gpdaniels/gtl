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

#include <container/any>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(any, traits, standard) {
    REQUIRE(std::is_pod<gtl::any>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::any>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::any>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::any>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(any, constructor, empty) {
    gtl::any any;
    testbench::do_not_optimise_away(any);
}

TEST(any, constructor, value) {
    gtl::any any(1);
    testbench::do_not_optimise_away(any);
}

TEST(any, evaluate, any) {
    gtl::any any(1);
    REQUIRE(static_cast<int>(any) == 1, "Unexpected value '%d' in any, expected %d.", static_cast<int>(any), 1);

    any = 1.0;
    REQUIRE(static_cast<int>(any) != 1, "Unexpected value '%d' in any, expected %f.", static_cast<int>(any), 1.0);
    REQUIRE(static_cast<double>(any) == 1.0,  "Unexpected value '%f' in any, expected %f.", static_cast<double>(any), 1.0);

    const char* string = "Hello world!";
    any = string;
    REQUIRE(testbench::is_string_same(static_cast<const char*>(any), string) == true, "gtl::any = '%s', expected '%s'", static_cast<char*>(any), string);
}
