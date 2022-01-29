/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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

#include <math/big_integer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

bool check_power_of_two(gtl::big_integer big_unsigned, bool negative, signed long long int initial_offset, signed long long int exponent, signed long long int final_value);
bool check_power_of_two(gtl::big_integer big_unsigned, bool negative, signed long long int initial_offset, signed long long int exponent, signed long long int final_value) {
    if ((big_unsigned < 0) != negative) {
        return false;
    }
    if (initial_offset > 0) {
        big_unsigned -= initial_offset;
    }
    else if (initial_offset < 0) {
        big_unsigned += -initial_offset;
    }
    if (big_unsigned < 0) {
        big_unsigned = -big_unsigned;
    }
    for (signed long long int i = 0; i < exponent; ++i) {
        if (big_unsigned <= 1) {
            return false;
        }
        big_unsigned /= 2;
    }
    return (big_unsigned == final_value);
}

TEST(big_integer, traits, standard) {
    REQUIRE((std::is_pod<gtl::big_integer>::value == false));

    REQUIRE((std::is_trivial<gtl::big_integer>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::big_integer>::value == false));

    REQUIRE((std::is_standard_layout<gtl::big_integer>::value == true));
}

TEST(big_integer, constructor, empty) {
    gtl::big_integer big_integer;
    REQUIRE(big_integer == 0);
}

TEST(big_integer, constructor, value) {
    bool value_b = true;

    char value_c = (1 << 6) + 1;
    signed char value_sc = (1 << 6) + 1;
    signed short value_ss = (1 << 14) + 1;
    signed int value_si = (1 << 30) + 1;
    signed long long int value_sll = (1ll << 62) + 1;

    char value_c_negative = static_cast<char>(1u << 7);
    signed char value_sc_negative = static_cast<signed char>(1u << 7);
    signed short value_ss_negative = static_cast<signed short>(1u << 15);
    signed int value_si_negative = static_cast<signed int>(1u << 31);
    signed long long int value_sll_negative = static_cast<signed long long int>(1ull << 63);

    unsigned char value_uc = (1u << 7) + 1;
    unsigned short value_us = (1u << 15) + 1;
    unsigned int value_ui = (1u << 31) + 1;
    unsigned long long int value_ull = (1ull << 63) + 1;

    gtl::big_integer big_integer_b(value_b);
    REQUIRE(big_integer_b == value_b);

    gtl::big_integer big_integer_c(value_c);
    REQUIRE(big_integer_c == value_c);

    gtl::big_integer big_integer_sc(value_sc);
    REQUIRE(big_integer_sc == value_sc);

    gtl::big_integer big_integer_ss(value_ss);
    REQUIRE(big_integer_ss == value_ss);

    gtl::big_integer big_integer_si(value_si);
    REQUIRE(big_integer_si == value_si);

    gtl::big_integer big_integer_sll(value_sll);
    REQUIRE(big_integer_sll == value_sll);


    gtl::big_integer big_integer_c_negative(value_c_negative);
    REQUIRE(big_integer_c_negative == value_c_negative);

    gtl::big_integer big_integer_sc_negative(value_sc_negative);
    REQUIRE(big_integer_sc_negative == value_sc_negative);

    gtl::big_integer big_integer_ss_negative(value_ss_negative);
    REQUIRE(big_integer_ss_negative == value_ss_negative);

    gtl::big_integer big_integer_si_negative(value_si_negative);
    REQUIRE(big_integer_si_negative == value_si_negative);

    gtl::big_integer big_integer_sll_negative(value_sll_negative);
    REQUIRE(big_integer_sll_negative == value_sll_negative);


    gtl::big_integer big_integer_uc(value_uc);
    REQUIRE(big_integer_uc == value_uc);

    gtl::big_integer big_integer_us(value_us);
    REQUIRE(big_integer_us == value_us);

    gtl::big_integer big_integer_ui(value_ui);
    REQUIRE(big_integer_ui == value_ui);

    gtl::big_integer big_integer_ull(value_ull);
    REQUIRE(big_integer_ull == value_ull);
}

TEST(big_integer, constructor, string) {
    gtl::big_integer big_integer("123456", 6);
    REQUIRE(big_integer == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_integer big_integer_95(two_power_95, testbench::string_length(two_power_95));
    REQUIRE(check_power_of_two(big_integer_95, false, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333, false, 0, 333, 1));


    gtl::big_integer big_integer_negative("-123456", 7);
    REQUIRE(big_integer_negative == -123456);

    constexpr static const char* two_power_95_negative = "-39614081257132168796771975168";
    gtl::big_integer big_integer_95_negative(two_power_95_negative, testbench::string_length(two_power_95_negative));
    REQUIRE(check_power_of_two(big_integer_95_negative, true, 0, 95, 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 0, 333, 1));
}

TEST(big_integer, constructor, copy) {
    gtl::big_integer big_integer("123456", 6);
    gtl::big_integer big_integer_copy(big_integer);
    REQUIRE(big_integer_copy == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_integer big_integer_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_integer big_integer_95_copy(big_integer_95);
    REQUIRE(check_power_of_two(big_integer_95_copy, false, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_integer big_integer_333_copy(big_integer_333);
    REQUIRE(check_power_of_two(big_integer_333_copy, false, 0, 333, 1));

    gtl::big_integer big_integer_negative("-123456", 7);
    gtl::big_integer big_integer_copy_negative(big_integer_negative);
    REQUIRE(big_integer_copy_negative == -123456);

    constexpr static const char* two_power_95_negative = "-39614081257132168796771975168";
    gtl::big_integer big_integer_95_negative(two_power_95_negative, testbench::string_length(two_power_95_negative));
    gtl::big_integer big_integer_95_copy_negative(big_integer_95_negative);
    REQUIRE(check_power_of_two(big_integer_95_copy_negative, true, 0, 95, 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    gtl::big_integer big_integer_333_copy_negative(big_integer_333_negative);
    REQUIRE(check_power_of_two(big_integer_333_copy_negative, true, 0, 333, 1));
}

TEST(big_integer, constructor, move) {
    gtl::big_integer big_integer("123456", 6);
    gtl::big_integer big_integer_move(std::move(big_integer));
    REQUIRE(big_integer_move == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_integer big_integer_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_integer big_integer_95_move(std::move(big_integer_95));
    REQUIRE(check_power_of_two(big_integer_95_move, false, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_integer big_integer_333_move(std::move(big_integer_333));
    REQUIRE(check_power_of_two(big_integer_333_move, false, 0, 333, 1));

    gtl::big_integer big_integer_negative("-123456", 7);
    gtl::big_integer big_integer_move_negative(std::move(big_integer_negative));
    REQUIRE(big_integer_move_negative == -123456);

    constexpr static const char* two_power_95_negative = "-39614081257132168796771975168";
    gtl::big_integer big_integer_95_negative(two_power_95_negative, testbench::string_length(two_power_95_negative));
    gtl::big_integer big_integer_95_move_negative(std::move(big_integer_95_negative));
    REQUIRE(check_power_of_two(big_integer_95_move_negative, true, 0, 95, 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    gtl::big_integer big_integer_333_move_negative(std::move(big_integer_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_move_negative, true, 0, 333, 1));
}

TEST(big_integer, operator, copy) {
    gtl::big_integer big_integer("123456", 6);
    gtl::big_integer big_integer_copy = big_integer;
    REQUIRE(big_integer_copy == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_integer big_integer_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_integer big_integer_95_copy = big_integer_95;
    REQUIRE(check_power_of_two(big_integer_95_copy, false, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_integer big_integer_333_copy = big_integer_333;
    REQUIRE(check_power_of_two(big_integer_333_copy, false, 0, 333, 1));

    gtl::big_integer big_integer_negative("-123456", 7);
    gtl::big_integer big_integer_copy_negative = big_integer_negative;
    REQUIRE(big_integer_copy_negative == -123456);

    constexpr static const char* two_power_95_negative = "-39614081257132168796771975168";
    gtl::big_integer big_integer_95_negative(two_power_95_negative, testbench::string_length(two_power_95_negative));
    gtl::big_integer big_integer_95_copy_negative = big_integer_95_negative;
    REQUIRE(check_power_of_two(big_integer_95_copy_negative, true, 0, 95, 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    gtl::big_integer big_integer_333_copy_negative = big_integer_333_negative;
    REQUIRE(check_power_of_two(big_integer_333_copy_negative, true, 0, 333, 1));
}

TEST(big_integer, operator, move) {
    gtl::big_integer big_integer("123456", 6);
    gtl::big_integer big_integer_move = std::move(big_integer);
    REQUIRE(big_integer_move == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_integer big_integer_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_integer big_integer_95_move = std::move(big_integer_95);
    REQUIRE(check_power_of_two(big_integer_95_move, false, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_integer big_integer_333_move = std::move(big_integer_333);
    REQUIRE(check_power_of_two(big_integer_333_move, false, 0, 333, 1));

    gtl::big_integer big_integer_negative("-123456", 7);
    gtl::big_integer big_integer_move_negative = std::move(big_integer_negative);
    REQUIRE(big_integer_move_negative == -123456);

    constexpr static const char* two_power_95_negative = "-39614081257132168796771975168";
    gtl::big_integer big_integer_95_negative(two_power_95_negative, testbench::string_length(two_power_95_negative));
    gtl::big_integer big_integer_95_move_negative = std::move(big_integer_95_negative);
    REQUIRE(check_power_of_two(big_integer_95_move_negative, true, 0, 95, 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    gtl::big_integer big_integer_333_move_negative = std::move(big_integer_333_negative);
    REQUIRE(check_power_of_two(big_integer_333_move_negative, true, 0, 333, 1));
}

TEST(big_integer, operator, bool) {
    gtl::big_integer big_integer_true(true);
    gtl::big_integer big_integer_false(false);

    REQUIRE(big_integer_true);
    REQUIRE(!big_integer_false);

    gtl::big_integer big_integer_positive(1);
    gtl::big_integer big_integer_zero(0);
    gtl::big_integer big_integer_negative(-1);

    REQUIRE(big_integer_positive);
    REQUIRE(!big_integer_zero);
    REQUIRE(big_integer_negative);
}

TEST(big_integer, operator, increment_pre) {
    gtl::big_integer big_integer(123);
    REQUIRE(big_integer == 123);
    REQUIRE(++big_integer == 124);
    REQUIRE(big_integer == 124);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333, false, 0, 333, 1));
    REQUIRE(check_power_of_two(++big_integer_333, false, 1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333, false, 1, 333, 1));

    gtl::big_integer big_integer_negative(-123);
    REQUIRE(big_integer_negative == -123);
    REQUIRE(++big_integer_negative == -122);
    REQUIRE(big_integer_negative == -122);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 0, 333, 1));
    REQUIRE(check_power_of_two(++big_integer_333_negative, true, 1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 1, 333, 1));
}

TEST(big_integer, operator, increment_post) {
    gtl::big_integer big_integer(123);
    REQUIRE(big_integer == 123);
    REQUIRE(big_integer++ == 123);
    REQUIRE(big_integer == 124);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333++, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333, false, 1, 333, 1));

    gtl::big_integer big_integer_negative(-123);
    REQUIRE(big_integer_negative == -123);
    REQUIRE(big_integer_negative++ == -123);
    REQUIRE(big_integer_negative == -122);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative++, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 1, 333, 1));
}

TEST(big_integer, operator, decrement_pre) {
    gtl::big_integer big_integer(123);
    REQUIRE(big_integer == 123);
    REQUIRE(--big_integer == 122);
    REQUIRE(big_integer == 122);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333, false, 0, 333, 1));
    REQUIRE(check_power_of_two(--big_integer_333, false, -1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333, false, -1, 333, 1));

    gtl::big_integer big_integer_negative(-123);
    REQUIRE(big_integer_negative == -123);
    REQUIRE(--big_integer_negative == -124);
    REQUIRE(big_integer_negative == -124);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 0, 333, 1));
    REQUIRE(check_power_of_two(--big_integer_333_negative, true, -1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, -1, 333, 1));
}

TEST(big_integer, operator, decrement_post) {
    gtl::big_integer big_integer(123);
    REQUIRE(big_integer == 123);
    REQUIRE(big_integer-- == 123);
    REQUIRE(big_integer == 122);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333--, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333, false, -1, 333, 1));

    gtl::big_integer big_integer_negative(-123);
    REQUIRE(big_integer_negative == -123);
    REQUIRE(big_integer_negative-- == -123);
    REQUIRE(big_integer_negative == -124);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative--, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative, true, -1, 333, 1));
}

TEST(big_integer, operator, add) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs + big_integer_rhs == lhs + rhs);
    REQUIRE(big_integer_lhs + 1 == lhs + 1);
    REQUIRE(big_integer_rhs + 1 == rhs + 1);
    REQUIRE(gtl::big_integer(0) + big_integer_lhs == big_integer_lhs);
    REQUIRE(gtl::big_integer(0) + big_integer_rhs == big_integer_rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 + 1, false, 1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 + 333, false, 333, 333, 1));

    signed long long int lhs_negative= -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative + big_integer_rhs_negative == lhs_negative + rhs_negative);
    REQUIRE(big_integer_lhs_negative + 1 == lhs_negative + 1);
    REQUIRE(big_integer_rhs_negative + 1 == rhs_negative + 1);
    REQUIRE(gtl::big_integer(0) + big_integer_lhs_negative == big_integer_lhs_negative);
    REQUIRE(gtl::big_integer(0) + big_integer_rhs_negative == big_integer_rhs_negative);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative + 1, true, 1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative + 333, true, 333, 333, 1));
}

TEST(big_integer, operator, subtract) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs - big_integer_rhs == lhs - rhs);
    REQUIRE(big_integer_lhs - 1 == lhs - 1);
    REQUIRE(big_integer_rhs - 1 == rhs - 1);
    REQUIRE(big_integer_lhs - 0 == big_integer_lhs);
    REQUIRE(big_integer_rhs - 0 == big_integer_rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 - 1, false, -1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 - 333, false, -333, 333, 1));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative - big_integer_rhs_negative == lhs_negative - rhs_negative);
    REQUIRE(big_integer_lhs_negative - 1 == lhs_negative - 1);
    REQUIRE(big_integer_rhs_negative - 1 == rhs_negative - 1);
    REQUIRE(big_integer_lhs_negative - 0 == big_integer_lhs_negative);
    REQUIRE(big_integer_rhs_negative - 0 == big_integer_rhs_negative);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative - 1, true, -1, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative - 333, true, -333, 333, 1));
}

TEST(big_integer, operator, multiply) {
    signed long long int lhs = 1ull << 61;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs * big_integer_rhs == lhs * rhs);
    REQUIRE(big_integer_lhs * 1 == lhs * 1);
    REQUIRE(big_integer_rhs * 1 == rhs * 1);
    REQUIRE(gtl::big_integer(0) * big_integer_lhs == 0);
    REQUIRE(gtl::big_integer(0) * big_integer_rhs == 0);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 * 1, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 * 333, false, 0, 333, 333));

    signed long long int lhs_negative = -(1ll << 61);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative * big_integer_rhs_negative == lhs_negative * rhs_negative);
    REQUIRE(big_integer_lhs_negative * 1 == lhs_negative * 1);
    REQUIRE(big_integer_rhs_negative * 1 == rhs_negative * 1);
    REQUIRE(gtl::big_integer(0) * big_integer_lhs_negative == 0);
    REQUIRE(gtl::big_integer(0) * big_integer_rhs_negative == 0);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative * 1, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative * 333, true, 0, 333, 333));
}

TEST(big_integer, operator, divide) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs / big_integer_rhs == lhs / rhs);
    REQUIRE(big_integer_lhs / 1 == lhs / 1);
    REQUIRE(big_integer_rhs / 1 == rhs / 1);
    REQUIRE(gtl::big_integer(0) / big_integer_lhs == 0);
    REQUIRE(gtl::big_integer(0) / big_integer_rhs == 0);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 / 1, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 / 1024, false, 0, 333 - 10, 1));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative / big_integer_rhs_negative == lhs_negative / rhs_negative);
    REQUIRE(big_integer_lhs_negative / 1 == lhs_negative / 1);
    REQUIRE(big_integer_rhs_negative / 1 == rhs_negative / 1);
    REQUIRE(gtl::big_integer(0) / big_integer_lhs_negative == 0);
    REQUIRE(gtl::big_integer(0) / big_integer_rhs_negative == 0);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative / 1, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative / 1024, true, 0, 333 - 10, 1));
}

TEST(big_integer, operator, modulo) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs % big_integer_rhs == lhs % rhs);
    REQUIRE(big_integer_lhs % 1 == lhs % 1);
    REQUIRE(big_integer_rhs % 1 == rhs % 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 % 1, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 2, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 3, false, 2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 4, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 5, false, 2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 6, false, 2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 7, false, 1, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 8, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % 9, false, 8, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333 % (big_integer_333 + 1), false, 0, 333, 1));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative % big_integer_rhs_negative == lhs_negative % rhs_negative);
    REQUIRE(big_integer_lhs_negative % 1 == lhs_negative % 1);
    REQUIRE(big_integer_rhs_negative % 1 == rhs_negative % 1);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative % 1, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 2, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 3, true, -2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 4, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 5, true, -2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 6, true, -2, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 7, true, -1, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 8, false, 0, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % 9, true, -8, 0, 0));
    REQUIRE(check_power_of_two(big_integer_333_negative % (big_integer_333_negative - 1), true, 0, 333, 1));
}

TEST(big_integer, operator, bit_shift_left) {
    signed long long int lhs = 1ull << 61;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs << 1 == lhs << 1);
    REQUIRE(big_integer_rhs << 1 == rhs << 1);
    REQUIRE(big_integer_lhs << 0 == lhs);
    REQUIRE(big_integer_rhs << 0 == rhs);
    REQUIRE(big_integer_rhs << 60 == lhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 << 0, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 << 1, false, 0, 334, 1));
    REQUIRE(check_power_of_two(big_integer_333 << 2, false, 0, 335, 1));

    signed long long int lhs_negative = -(1ll << 61);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative << 1 == lhs_negative << 1);
    REQUIRE(big_integer_rhs_negative << 1 == rhs_negative << 1);
    REQUIRE(big_integer_lhs_negative << 0 == lhs_negative);
    REQUIRE(big_integer_rhs_negative << 0 == rhs_negative);
    REQUIRE(big_integer_rhs_negative << 60 == lhs_negative);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative << 0, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative << 1, true, 0, 334, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative << 2, true, 0, 335, 1));
}

TEST(big_integer, operator, bit_shift_right) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs >> 1 == lhs >> 1);
    REQUIRE(big_integer_rhs >> 1 == rhs >> 1);
    REQUIRE(big_integer_lhs >> 0 == lhs >> 0);
    REQUIRE(big_integer_rhs >> 0 == rhs >> 0);
    REQUIRE(big_integer_lhs >> 61 == rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_integer_333 >> 0, false, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333 >> 1, false, 0, 332, 1));
    REQUIRE(check_power_of_two(big_integer_333 >> 2, false, 0, 331, 1));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative >> 1 == lhs_negative >> 1);
    REQUIRE(big_integer_rhs_negative >> 1 == rhs_negative >> 1);
    REQUIRE(big_integer_lhs_negative >> 0 == lhs_negative >> 0);
    REQUIRE(big_integer_rhs_negative >> 0 == rhs_negative >> 0);
    REQUIRE(big_integer_lhs_negative >> 61 == rhs_negative);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(check_power_of_two(big_integer_333_negative >> 0, true, 0, 333, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative >> 1, true, 0, 332, 1));
    REQUIRE(check_power_of_two(big_integer_333_negative >> 2, true, 0, 331, 1));
}

TEST(big_integer, operator, bit_and) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs & big_integer_rhs) == 0);
    REQUIRE((big_integer_lhs & big_integer_lhs) == big_integer_lhs);
    REQUIRE((big_integer_rhs & big_integer_rhs) == big_integer_rhs);
    REQUIRE((big_integer_lhs & 2) == 0);
    REQUIRE((big_integer_rhs & 2) == 2);
    REQUIRE((big_integer_lhs & 1) == 0);
    REQUIRE((big_integer_rhs & 1) == 0);
    REQUIRE((big_integer_lhs & 0) == 0);
    REQUIRE((big_integer_rhs & 0) == 0);
    REQUIRE(((big_integer_lhs - 1) & big_integer_lhs) == 0);
    REQUIRE(((big_integer_lhs - 1) & big_integer_rhs) == rhs);
    REQUIRE(((big_integer_lhs - 1) & 1) == 1);
    REQUIRE(((big_integer_rhs - 1) & 1) == 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_333 & big_integer_333) == big_integer_333);
    REQUIRE((big_integer_333 & 1) == 0);
    REQUIRE((big_integer_333 & 0) == 0);
    REQUIRE(((big_integer_333 - 1) & big_integer_333) == 0);
    REQUIRE(((big_integer_333 - 1) & big_integer_lhs) == lhs);
    REQUIRE(((big_integer_333 - 1) & big_integer_rhs) == rhs);
    REQUIRE(((big_integer_333 - 1) & 1) == 1);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative & big_integer_rhs_negative) == big_integer_lhs_negative);
    REQUIRE((big_integer_lhs_negative & big_integer_lhs_negative) == big_integer_lhs_negative);
    REQUIRE((big_integer_rhs_negative & big_integer_rhs_negative) == big_integer_rhs_negative);
    REQUIRE((big_integer_lhs_negative & 2) == 0);
    REQUIRE((big_integer_rhs_negative & 2) == 2);
    REQUIRE((big_integer_lhs_negative & 1) == 0);
    REQUIRE((big_integer_rhs_negative & 1) == 0);
    REQUIRE((big_integer_lhs_negative & 0) == 0);
    REQUIRE((big_integer_rhs_negative & 0) == 0);
    REQUIRE(((big_integer_lhs_negative - 1) & big_integer_lhs_negative) == big_integer_lhs_negative + big_integer_lhs_negative);
    REQUIRE(((big_integer_lhs_negative - 1) & big_integer_rhs_negative) == lhs_negative - 2);
    REQUIRE(((big_integer_lhs_negative - 1) & 1) == 1);
    REQUIRE(((big_integer_rhs_negative - 1) & 1) == 1);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_333_negative & big_integer_333_negative) == big_integer_333_negative);
    REQUIRE((big_integer_333_negative & 1) == 0);
    REQUIRE((big_integer_333_negative & 0) == 0);
    REQUIRE(((big_integer_333_negative - 1) & big_integer_333_negative) == big_integer_333_negative + big_integer_333_negative);
    REQUIRE(((big_integer_333_negative - 1) & big_integer_lhs_negative) == big_integer_333_negative + big_integer_lhs_negative);
    REQUIRE(((big_integer_333_negative - 1) & big_integer_rhs_negative) == big_integer_333_negative - 2);
    REQUIRE(((big_integer_333_negative - 1) & 1) == 1);
}

TEST(big_integer, operator, bit_or) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs | big_integer_rhs) == (lhs | rhs));
    REQUIRE((big_integer_lhs | big_integer_lhs) == big_integer_lhs);
    REQUIRE((big_integer_rhs | big_integer_rhs) == big_integer_rhs);
    REQUIRE((big_integer_lhs | 1) == (lhs | 1));
    REQUIRE((big_integer_rhs | 1) == (rhs | 1));
    REQUIRE((big_integer_lhs | 0) == lhs);
    REQUIRE((big_integer_rhs | 0) == rhs);
    REQUIRE(((big_integer_lhs - 1) | big_integer_lhs) == ((lhs - 1) | lhs));
    REQUIRE(((big_integer_lhs - 1) | big_integer_rhs) == ((lhs - 1) | rhs));
    REQUIRE(((big_integer_lhs - 1) | 1) == ((lhs - 1) | 1));
    REQUIRE(((big_integer_rhs - 1) | 1) == ((rhs - 1) | 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_333 | big_integer_333) == big_integer_333);
    REQUIRE((big_integer_333 | 1) == (gtl::big_integer(1) | big_integer_333));
    REQUIRE((big_integer_333 | 0) == big_integer_333);
    REQUIRE(((big_integer_333 - 1) | big_integer_333) == (big_integer_333 | (big_integer_333 - 1)));
    REQUIRE(((big_integer_333 - 1) | big_integer_lhs) == (big_integer_333 - 1));
    REQUIRE(((big_integer_333 - 1) | big_integer_rhs) == (big_integer_333 - 1));
    REQUIRE(((big_integer_333 - 1) | 1) == (big_integer_333 - 1));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative | big_integer_rhs_negative) == (lhs_negative | rhs_negative));
    REQUIRE((big_integer_lhs_negative | big_integer_lhs_negative) == big_integer_lhs_negative);
    REQUIRE((big_integer_rhs_negative | big_integer_rhs_negative) == big_integer_rhs_negative);
    REQUIRE((big_integer_lhs_negative | 1) == (lhs_negative | 1));
    REQUIRE((big_integer_rhs_negative | 1) == (rhs_negative | 1));
    REQUIRE((big_integer_lhs_negative | 0) == lhs_negative);
    REQUIRE((big_integer_rhs_negative | 0) == rhs_negative);
    REQUIRE(((big_integer_lhs_negative - 1) | big_integer_lhs_negative) == ((lhs_negative - 1) | lhs_negative));
    REQUIRE(((big_integer_lhs_negative - 1) | big_integer_rhs_negative) == ((lhs_negative - 1) | rhs_negative));
    REQUIRE(((big_integer_lhs_negative - 1) | 1) == ((lhs_negative - 1) | 1));
    REQUIRE(((big_integer_rhs_negative - 1) | 1) == ((rhs_negative - 1) | 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_333_negative | big_integer_333_negative) == big_integer_333_negative);
    REQUIRE((big_integer_333_negative | 1) == (gtl::big_integer(1) | big_integer_333_negative));
    REQUIRE((big_integer_333_negative | 0) == big_integer_333_negative);
    REQUIRE(((big_integer_333_negative - 1) | big_integer_333_negative) == (big_integer_333_negative | (big_integer_333_negative - 1)));
    REQUIRE(((big_integer_333_negative + 1) | big_integer_lhs_negative) == (big_integer_lhs_negative + 1));
    REQUIRE(((big_integer_333_negative + 1) | big_integer_rhs_negative) == (big_integer_rhs_negative + 1));
    REQUIRE(((big_integer_333_negative - 1) | 1) == (big_integer_333_negative - 1));
}

TEST(big_integer, operator, bit_xor) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs ^ big_integer_rhs) == (lhs ^ rhs));
    REQUIRE((big_integer_lhs ^ big_integer_lhs) == 0);
    REQUIRE((big_integer_rhs ^ big_integer_rhs) == 0);
    REQUIRE((big_integer_lhs ^ 1) == (lhs ^ 1));
    REQUIRE((big_integer_rhs ^ 1) == (rhs ^ 1));
    REQUIRE((big_integer_lhs ^ 0) == lhs);
    REQUIRE((big_integer_rhs ^ 0) == rhs);
    REQUIRE(((big_integer_lhs - 1) ^ big_integer_lhs) == ((lhs - 1) ^ lhs));
    REQUIRE(((big_integer_lhs - 1) ^ big_integer_rhs) == ((lhs - 1) ^ rhs));
    REQUIRE(((big_integer_lhs - 1) ^ 1) == ((lhs - 1) ^ 1));
    REQUIRE(((big_integer_rhs - 1) ^ 1) == ((rhs - 1) ^ 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_333 ^ big_integer_333) == 0);
    REQUIRE((big_integer_333 ^ 1) == (gtl::big_integer(1) ^ big_integer_333));
    REQUIRE((big_integer_333 ^ 0) == big_integer_333);
    REQUIRE(((big_integer_333 - 1) ^ big_integer_333) == (big_integer_333 ^ (big_integer_333 - 1)));
    REQUIRE(((big_integer_333 - 1) ^ big_integer_lhs) == (big_integer_lhs ^ (big_integer_333 - 1)));
    REQUIRE(((big_integer_333 - 1) ^ big_integer_rhs) == (big_integer_rhs ^ (big_integer_333 - 1)));
    REQUIRE(((big_integer_333 - 1) ^ 1) == (gtl::big_integer(1) ^ (big_integer_333 - 1)));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative ^ big_integer_rhs_negative) == (lhs_negative ^ rhs_negative));
    REQUIRE((big_integer_lhs_negative ^ big_integer_lhs_negative) == 0);
    REQUIRE((big_integer_rhs_negative ^ big_integer_rhs_negative) == 0);
    REQUIRE((big_integer_lhs_negative ^ 1) == (lhs_negative ^ 1));
    REQUIRE((big_integer_rhs_negative ^ 1) == (rhs_negative ^ 1));
    REQUIRE((big_integer_lhs_negative ^ 0) == lhs_negative);
    REQUIRE((big_integer_rhs_negative ^ 0) == rhs_negative);
    REQUIRE(((big_integer_lhs_negative - 1) ^ big_integer_lhs_negative) == ((lhs_negative - 1) ^ lhs_negative));
    REQUIRE(((big_integer_lhs_negative - 1) ^ big_integer_rhs_negative) == ((lhs_negative - 1) ^ rhs_negative));
    REQUIRE(((big_integer_lhs_negative - 1) ^ 1) == ((lhs_negative - 1) ^ 1));
    REQUIRE(((big_integer_rhs_negative - 1) ^ 1) == ((rhs_negative - 1) ^ 1));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_333_negative ^ big_integer_333_negative) == 0);
    REQUIRE((big_integer_333_negative ^ 1) == (gtl::big_integer(1) ^ big_integer_333_negative));
    REQUIRE((big_integer_333_negative ^ 0) == big_integer_333_negative);
    REQUIRE(((big_integer_333_negative - 1) ^ big_integer_333_negative) == (big_integer_333_negative ^ (big_integer_333_negative - 1)));
    REQUIRE(((big_integer_333_negative - 1) ^ big_integer_lhs_negative) == (big_integer_lhs_negative ^ (big_integer_333_negative - 1)));
    REQUIRE(((big_integer_333_negative - 1) ^ big_integer_rhs_negative) == (big_integer_rhs_negative ^ (big_integer_333_negative - 1)));
    REQUIRE(((big_integer_333_negative - 1) ^ 1) == (gtl::big_integer(1) ^ (big_integer_333_negative - 1)));
}

TEST(big_integer, operator, bit_not) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(~big_integer_lhs == ~lhs);
    REQUIRE(~big_integer_rhs == ~static_cast<signed int>(rhs));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    constexpr static const char* two_power_352 = "9173994463960286046443283581208347763186259956673124494950355357547691504353939232280074212440502746218496";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_integer big_integer_352(two_power_352, testbench::string_length(two_power_352));
    REQUIRE(~big_integer_333 == -big_integer_333 - 1);
    REQUIRE(~(big_integer_352 - 1) == -big_integer_352);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(~big_integer_lhs_negative == ~lhs_negative);
    REQUIRE(~big_integer_rhs_negative == ~static_cast<signed int>(rhs_negative));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    constexpr static const char* two_power_352_negative = "-9173994463960286046443283581208347763186259956673124494950355357547691504353939232280074212440502746218496";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    gtl::big_integer big_integer_352_negative(two_power_352_negative, testbench::string_length(two_power_352_negative));
    REQUIRE(~big_integer_333_negative == -big_integer_333_negative - 1);
    REQUIRE(~(big_integer_352_negative - 1) == -big_integer_352_negative);
}

TEST(big_integer, operator, less_than) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs < big_integer_rhs) == false);
    REQUIRE((big_integer_rhs < big_integer_lhs) == true);
    REQUIRE((big_integer_lhs < big_integer_lhs) == false);
    REQUIRE((big_integer_rhs < big_integer_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs < big_integer_333) == true);
    REQUIRE((big_integer_rhs < big_integer_333) == true);
    REQUIRE((big_integer_333 < big_integer_lhs) == false);
    REQUIRE((big_integer_333 < big_integer_rhs) == false);
    REQUIRE((big_integer_333 < big_integer_333) == false);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative < big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative < big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative < big_integer_lhs_negative) == false);
    REQUIRE((big_integer_rhs_negative < big_integer_rhs_negative) == false);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative < big_integer_333_negative) == false);
    REQUIRE((big_integer_rhs_negative < big_integer_333_negative) == false);
    REQUIRE((big_integer_333_negative < big_integer_lhs_negative) == true);
    REQUIRE((big_integer_333_negative < big_integer_rhs_negative) == true);
    REQUIRE((big_integer_333_negative < big_integer_333_negative) == false);

    REQUIRE((big_integer_lhs < big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative < big_integer_lhs) == true);
    REQUIRE((big_integer_rhs < big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative < big_integer_rhs) == true);
}

TEST(big_integer, operator, greater_than) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs > big_integer_rhs) == true);
    REQUIRE((big_integer_rhs > big_integer_lhs) == false);
    REQUIRE((big_integer_lhs > big_integer_lhs) == false);
    REQUIRE((big_integer_rhs > big_integer_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs > big_integer_333) == false);
    REQUIRE((big_integer_rhs > big_integer_333) == false);
    REQUIRE((big_integer_333 > big_integer_lhs) == true);
    REQUIRE((big_integer_333 > big_integer_rhs) == true);
    REQUIRE((big_integer_333 > big_integer_333) == false);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative > big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative > big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative > big_integer_lhs_negative) == false);
    REQUIRE((big_integer_rhs_negative > big_integer_rhs_negative) == false);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative > big_integer_333_negative) == true);
    REQUIRE((big_integer_rhs_negative > big_integer_333_negative) == true);
    REQUIRE((big_integer_333_negative > big_integer_lhs_negative) == false);
    REQUIRE((big_integer_333_negative > big_integer_rhs_negative) == false);
    REQUIRE((big_integer_333_negative > big_integer_333_negative) == false);

    REQUIRE((big_integer_lhs > big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative > big_integer_lhs) == false);
    REQUIRE((big_integer_rhs > big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative > big_integer_rhs) == false);
}

TEST(big_integer, operator, less_than_or_equal_to) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs <= big_integer_rhs) == false);
    REQUIRE((big_integer_rhs <= big_integer_lhs) == true);
    REQUIRE((big_integer_lhs <= big_integer_lhs) == true);
    REQUIRE((big_integer_rhs <= big_integer_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs <= big_integer_333) == true);
    REQUIRE((big_integer_rhs <= big_integer_333) == true);
    REQUIRE((big_integer_333 <= big_integer_lhs) == false);
    REQUIRE((big_integer_333 <= big_integer_rhs) == false);
    REQUIRE((big_integer_333 <= big_integer_333) == true);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative <= big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative <= big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative <= big_integer_lhs_negative) == true);
    REQUIRE((big_integer_rhs_negative <= big_integer_rhs_negative) == true);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative <= big_integer_333_negative) == false);
    REQUIRE((big_integer_rhs_negative <= big_integer_333_negative) == false);
    REQUIRE((big_integer_333_negative <= big_integer_lhs_negative) == true);
    REQUIRE((big_integer_333_negative <= big_integer_rhs_negative) == true);
    REQUIRE((big_integer_333_negative <= big_integer_333_negative) == true);

    REQUIRE((big_integer_lhs <= big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative <= big_integer_lhs) == true);
    REQUIRE((big_integer_rhs <= big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative <= big_integer_rhs) == true);
}

TEST(big_integer, operator, greater_than_or_equal_to) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs >= big_integer_rhs) == true);
    REQUIRE((big_integer_rhs >= big_integer_lhs) == false);
    REQUIRE((big_integer_lhs >= big_integer_lhs) == true);
    REQUIRE((big_integer_rhs >= big_integer_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs >= big_integer_333) == false);
    REQUIRE((big_integer_rhs >= big_integer_333) == false);
    REQUIRE((big_integer_333 >= big_integer_lhs) == true);
    REQUIRE((big_integer_333 >= big_integer_rhs) == true);
    REQUIRE((big_integer_333 >= big_integer_333) == true);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative >= big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative >= big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative >= big_integer_lhs_negative) == true);
    REQUIRE((big_integer_rhs_negative >= big_integer_rhs_negative) == true);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative >= big_integer_333_negative) == true);
    REQUIRE((big_integer_rhs_negative >= big_integer_333_negative) == true);
    REQUIRE((big_integer_333_negative >= big_integer_lhs_negative) == false);
    REQUIRE((big_integer_333_negative >= big_integer_rhs_negative) == false);
    REQUIRE((big_integer_333_negative >= big_integer_333_negative) == true);

    REQUIRE((big_integer_lhs >= big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative >= big_integer_lhs) == false);
    REQUIRE((big_integer_rhs >= big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative >= big_integer_rhs) == false);
}

TEST(big_integer, operator, equal_to) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs == big_integer_rhs) == false);
    REQUIRE((big_integer_rhs == big_integer_lhs) == false);
    REQUIRE((big_integer_lhs == big_integer_lhs) == true);
    REQUIRE((big_integer_rhs == big_integer_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs == big_integer_333) == false);
    REQUIRE((big_integer_rhs == big_integer_333) == false);
    REQUIRE((big_integer_333 == big_integer_lhs) == false);
    REQUIRE((big_integer_333 == big_integer_rhs) == false);
    REQUIRE((big_integer_333 == big_integer_333) == true);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative == big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative == big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative == big_integer_lhs_negative) == true);
    REQUIRE((big_integer_rhs_negative == big_integer_rhs_negative) == true);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative == big_integer_333_negative) == false);
    REQUIRE((big_integer_rhs_negative == big_integer_333_negative) == false);
    REQUIRE((big_integer_333_negative == big_integer_lhs_negative) == false);
    REQUIRE((big_integer_333_negative == big_integer_rhs_negative) == false);
    REQUIRE((big_integer_333_negative == big_integer_333_negative) == true);

    REQUIRE((big_integer_lhs == big_integer_lhs_negative) == false);
    REQUIRE((big_integer_lhs_negative == big_integer_lhs) == false);
    REQUIRE((big_integer_rhs == big_integer_rhs_negative) == false);
    REQUIRE((big_integer_rhs_negative == big_integer_rhs) == false);
}

TEST(big_integer, operator, not_equal_to) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE((big_integer_lhs != big_integer_rhs) == true);
    REQUIRE((big_integer_rhs != big_integer_lhs) == true);
    REQUIRE((big_integer_lhs != big_integer_lhs) == false);
    REQUIRE((big_integer_rhs != big_integer_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_integer_lhs != big_integer_333) == true);
    REQUIRE((big_integer_rhs != big_integer_333) == true);
    REQUIRE((big_integer_333 != big_integer_lhs) == true);
    REQUIRE((big_integer_333 != big_integer_rhs) == true);
    REQUIRE((big_integer_333 != big_integer_333) == false);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE((big_integer_lhs_negative != big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative != big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative != big_integer_lhs_negative) == false);
    REQUIRE((big_integer_rhs_negative != big_integer_rhs_negative) == false);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE((big_integer_lhs_negative != big_integer_333_negative) == true);
    REQUIRE((big_integer_rhs_negative != big_integer_333_negative) == true);
    REQUIRE((big_integer_333_negative != big_integer_lhs_negative) == true);
    REQUIRE((big_integer_333_negative != big_integer_rhs_negative) == true);
    REQUIRE((big_integer_333_negative != big_integer_333_negative) == false);

    REQUIRE((big_integer_lhs != big_integer_lhs_negative) == true);
    REQUIRE((big_integer_lhs_negative != big_integer_lhs) == true);
    REQUIRE((big_integer_rhs != big_integer_rhs_negative) == true);
    REQUIRE((big_integer_rhs_negative != big_integer_rhs) == true);
}

TEST(big_integer, function, signum) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(gtl::big_integer::signum(big_integer_lhs) == 1);
    REQUIRE(gtl::big_integer::signum(big_integer_rhs) == 1);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(gtl::big_integer::signum(big_integer_lhs_negative) == -1);
    REQUIRE(gtl::big_integer::signum(big_integer_rhs_negative) == -1);

    REQUIRE(gtl::big_integer::signum(0) == 0);
}

TEST(big_integer, function, abs) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(gtl::big_integer::abs(big_integer_lhs) == lhs);
    REQUIRE(gtl::big_integer::abs(big_integer_rhs) == rhs);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(gtl::big_integer::abs(big_integer_lhs_negative) == lhs);
    REQUIRE(gtl::big_integer::abs(big_integer_rhs_negative) == rhs);

    REQUIRE(gtl::big_integer::signum(0) == 0);
}

TEST(big_integer, operator, get_length_bits) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs.get_length_bits() == 64);
    REQUIRE(big_integer_rhs.get_length_bits() == 32);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_integer_333.get_length_bits() == 352);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative.get_length_bits() == 64);
    REQUIRE(big_integer_rhs_negative.get_length_bits() == 32);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(big_integer_333_negative.get_length_bits() == 352);
}

TEST(big_integer, operator, get_length_bytes) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs.get_length_bytes() == 8);
    REQUIRE(big_integer_rhs.get_length_bytes() == 4);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_integer_333.get_length_bytes() == 44);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative.get_length_bytes() == 8);
    REQUIRE(big_integer_rhs_negative.get_length_bytes() == 4);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(big_integer_333_negative.get_length_bytes() == 44);
}

TEST(big_integer, operator, get_length_decimal) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs.get_length_decimal() == 19);
    REQUIRE(big_integer_rhs.get_length_decimal() == 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_integer_333.get_length_decimal() == 101);

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative.get_length_decimal() == 19);
    REQUIRE(big_integer_rhs_negative.get_length_decimal() == 1);

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(big_integer_333_negative.get_length_decimal() == 101);
}

TEST(big_integer, operator, to_string) {
    signed long long int lhs = 1ull << 62;
    signed long long int rhs = 2;

    char string_lhs[21];
    char string_rhs[3];
    gtl::big_integer big_integer_lhs(lhs);
    gtl::big_integer big_integer_rhs(rhs);
    REQUIRE(big_integer_lhs.to_string(string_lhs, 21) == 19);
    REQUIRE(big_integer_rhs.to_string(string_rhs, 3) == 1);
    REQUIRE(testbench::is_string_same(string_lhs, "4611686018427387904"));
    REQUIRE(testbench::is_string_same(string_rhs, "2"));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    char string_333[103];
    gtl::big_integer big_integer_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_integer_333.to_string(string_333, 103) == 101);
    REQUIRE(testbench::is_string_same(string_333, two_power_333));

    signed long long int lhs_negative = -(1ll << 62);
    signed long long int rhs_negative = -2;

    char string_lhs_negative[21];
    char string_rhs_negative[3];
    gtl::big_integer big_integer_lhs_negative(lhs_negative);
    gtl::big_integer big_integer_rhs_negative(rhs_negative);
    REQUIRE(big_integer_lhs_negative.to_string(string_lhs_negative, 21) == 20);
    REQUIRE(big_integer_rhs_negative.to_string(string_rhs_negative, 3) == 2);
    REQUIRE(testbench::is_string_same(string_lhs_negative, "-4611686018427387904"));
    REQUIRE(testbench::is_string_same(string_rhs_negative, "-2"));

    constexpr static const char* two_power_333_negative = "-17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    char string_333_negative[103];
    gtl::big_integer big_integer_333_negative(two_power_333_negative, testbench::string_length(two_power_333_negative));
    REQUIRE(big_integer_333_negative.to_string(string_333_negative, 103) == 102);
    REQUIRE(testbench::is_string_same(string_333_negative, two_power_333_negative));
}
