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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <string/static_string>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

// TODO: Broken on windows! Crashes the compiler...
#if !defined(_MSC_VER)

TEST(static_string, constructor, empty) {
    gtl::static_string static_string;
    testbench::do_not_optimise_away(static_string);
}

TEST(static_string, constructor, string) {
    gtl::static_string static_string = GTL_STATIC_STRING("String");
    testbench::do_not_optimise_away(static_string);
}

TEST(static_string, function, size) {
    gtl::static_string static_string = GTL_STATIC_STRING("String");
    REQUIRE(static_string.size() == 6);
}

TEST(static_string, function, length) {
    gtl::static_string static_string = GTL_STATIC_STRING("String");
    REQUIRE(static_string.length() == 6);
}

TEST(static_string, function, c_str) {
    gtl::static_string static_string = GTL_STATIC_STRING("String");
    REQUIRE(testbench::is_string_same(static_string.c_str(), "String"));
}

TEST(static_string, function, append) {
    gtl::static_string static_string_lhs = GTL_STATIC_STRING("Hello ");
    gtl::static_string static_string_rhs = GTL_STATIC_STRING("World!");
    gtl::static_string static_string_append = static_string_lhs.append(static_string_rhs);
    gtl::static_string static_string_operator = static_string_lhs + static_string_rhs;
    REQUIRE(testbench::is_string_same(static_string_append.c_str(), "Hello World!"));
    REQUIRE(testbench::is_string_same(static_string_operator.c_str(), "Hello World!"));
}

TEST(static_string, function, find_first) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    REQUIRE(static_string.find_first('r') == 2);
    REQUIRE(static_string.find_first('r', 0) == 2);
    REQUIRE(static_string.find_first('r', 1) == 2);
    REQUIRE(static_string.find_first('r', 2) == 2);
    REQUIRE(static_string.find_first('r', 3) == 8);
    REQUIRE(static_string.find_first('r', 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('r', 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('r', -1) == 2);

    REQUIRE(static_string.find_first('\0') == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 2) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 3) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first('\0', -1) == 0xFFFFFFFFFFFFFFFF);

    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r")) == 2);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 0) == 2);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 1) == 2);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 2) == 2);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 3) == 8);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("r"), -1) == 2);

    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X")) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 2) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 3) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_first(GTL_STATIC_STRING("X"), -1) == 0xFFFFFFFFFFFFFFFF);
}

TEST(static_string, function, find_last) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    REQUIRE(static_string.find_last('r') == 8);
    REQUIRE(static_string.find_last('r', 13) == 8);
    REQUIRE(static_string.find_last('r', 12) == 8);
    REQUIRE(static_string.find_last('r', 11) == 8);
    REQUIRE(static_string.find_last('r', 10) == 8);
    REQUIRE(static_string.find_last('r', 9) == 8);
    REQUIRE(static_string.find_last('r', 8) == 8);
    REQUIRE(static_string.find_last('r', 7) == 2);
    REQUIRE(static_string.find_last('r', 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('r', 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('r', -1) == 0xFFFFFFFFFFFFFFFF);

    REQUIRE(static_string.find_last('\0') == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 13) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 12) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 11) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 10) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 8) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 7) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last('\0', -1) == 0xFFFFFFFFFFFFFFFF);

    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r")) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 13) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 12) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 11) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 10) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 9) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 8) == 8);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 7) == 2);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("r"), -1) == 0xFFFFFFFFFFFFFFFF);

    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X")) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 13) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 12) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 11) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 10) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 9) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 8) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 7) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 1) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 0) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), 100) == 0xFFFFFFFFFFFFFFFF);
    REQUIRE(static_string.find_last(GTL_STATIC_STRING("X"), -1) == 0xFFFFFFFFFFFFFFFF);
}

TEST(static_string, function, sub_string) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    gtl::static_string static_sub_string = static_string.sub_string<3,6>();
    REQUIRE(testbench::is_string_same(static_sub_string.c_str(), "ingStr"));
}

TEST(static_string, function, to_lower) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    gtl::static_string static_string_lower = static_string.to_lower();
    REQUIRE(testbench::is_string_same(static_string_lower.c_str(), "stringstring"));
}

TEST(static_string, function, to_upper) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    gtl::static_string static_string_upper = static_string.to_upper();
    REQUIRE(testbench::is_string_same(static_string_upper.c_str(), "STRINGSTRING"));
}

TEST(static_string, function, replace) {
    gtl::static_string static_string = GTL_STATIC_STRING("StringString");
    gtl::static_string static_string_replaced = static_string.replace<'S', 'P'>();
    REQUIRE(testbench::is_string_same(static_string_replaced.c_str(), "PtringPtring"));
}

TEST(static_string, function, operators) {
    gtl::static_string static_string_X = GTL_STATIC_STRING("");

    gtl::static_string static_string_a = GTL_STATIC_STRING("a");
    gtl::static_string static_string_b = GTL_STATIC_STRING("b");
    gtl::static_string static_string_c = GTL_STATIC_STRING("c");

    // ==
    REQUIRE((static_string_a == static_string_X) == false);
    REQUIRE((static_string_X == static_string_a) == false);
    REQUIRE((static_string_a == static_string_a) == true);

    REQUIRE((static_string_a == static_string_b) == false);
    REQUIRE((static_string_a == static_string_c) == false);
    REQUIRE((static_string_b == static_string_c) == false);

    REQUIRE((static_string_b == static_string_a) == false);
    REQUIRE((static_string_c == static_string_a) == false);
    REQUIRE((static_string_c == static_string_b) == false);

    // !=
    REQUIRE((static_string_a != static_string_X) == true);
    REQUIRE((static_string_X != static_string_a) == true);
    REQUIRE((static_string_a != static_string_a) == false);

    REQUIRE((static_string_a != static_string_b) == true);
    REQUIRE((static_string_a != static_string_c) == true);
    REQUIRE((static_string_b != static_string_c) == true);

    REQUIRE((static_string_b != static_string_a) == true);
    REQUIRE((static_string_c != static_string_a) == true);
    REQUIRE((static_string_c != static_string_b) == true);

    // <
    REQUIRE((static_string_a < static_string_X) == false);
    REQUIRE((static_string_X < static_string_a) == true);
    REQUIRE((static_string_a < static_string_a) == false);

    REQUIRE((static_string_a < static_string_b) == true);
    REQUIRE((static_string_a < static_string_c) == true);
    REQUIRE((static_string_b < static_string_c) == true);

    REQUIRE((static_string_b < static_string_a) == false);
    REQUIRE((static_string_c < static_string_a) == false);
    REQUIRE((static_string_c < static_string_b) == false);

    // >
    REQUIRE((static_string_a > static_string_X) == true);
    REQUIRE((static_string_X > static_string_a) == false);
    REQUIRE((static_string_a > static_string_a) == false);

    REQUIRE((static_string_a > static_string_b) == false);
    REQUIRE((static_string_a > static_string_c) == false);
    REQUIRE((static_string_b > static_string_c) == false);

    REQUIRE((static_string_b > static_string_a) == true);
    REQUIRE((static_string_c > static_string_a) == true);
    REQUIRE((static_string_c > static_string_b) == true);

    // <=
    REQUIRE((static_string_a <= static_string_X) == false);
    REQUIRE((static_string_X <= static_string_a) == true);
    REQUIRE((static_string_a <= static_string_a) == true);

    REQUIRE((static_string_a <= static_string_b) == true);
    REQUIRE((static_string_a <= static_string_c) == true);
    REQUIRE((static_string_b <= static_string_c) == true);

    REQUIRE((static_string_b <= static_string_a) == false);
    REQUIRE((static_string_c <= static_string_a) == false);
    REQUIRE((static_string_c <= static_string_b) == false);

    // >=
    REQUIRE((static_string_a >= static_string_X) == true);
    REQUIRE((static_string_X >= static_string_a) == false);
    REQUIRE((static_string_a >= static_string_a) == true);

    REQUIRE((static_string_a >= static_string_b) == false);
    REQUIRE((static_string_a >= static_string_c) == false);
    REQUIRE((static_string_b >= static_string_c) == false);

    REQUIRE((static_string_b >= static_string_a) == true);
    REQUIRE((static_string_c >= static_string_a) == true);
    REQUIRE((static_string_c >= static_string_b) == true);
}

#endif