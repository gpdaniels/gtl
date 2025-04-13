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

#include <testbench/comparison.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <math/big_unsigned>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

bool check_power_of_two(gtl::big_unsigned big_unsigned, signed long long int initial_offset, unsigned long long int exponent, unsigned long long int final_value);

bool check_power_of_two(gtl::big_unsigned big_unsigned, signed long long int initial_offset, unsigned long long int exponent, unsigned long long int final_value) {
    if (initial_offset > 0) {
        big_unsigned -= initial_offset;
    }
    else if (initial_offset < 0) {
        big_unsigned += -initial_offset;
    }
    for (unsigned long long int i = 0; i < exponent; ++i) {
        if (big_unsigned <= 1) {
            return false;
        }
        big_unsigned /= 2;
    }
    return (big_unsigned == final_value);
}

TEST(big_unsigned, traits, standard) {
    REQUIRE((std::is_pod<gtl::big_unsigned>::value == false));

    REQUIRE((std::is_trivial<gtl::big_unsigned>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::big_unsigned>::value == false));

    REQUIRE((std::is_standard_layout<gtl::big_unsigned>::value == true));
}

TEST(big_unsigned, constructor, empty) {
    gtl::big_unsigned big_unsigned;
    REQUIRE(big_unsigned == 0);
}

TEST(big_unsigned, constructor, value) {
    bool value_b = true;
    char value_c = (1 << 6) + 1;
    signed char value_sc = (1 << 6) + 1;
    signed short value_ss = (1 << 14) + 1;
    signed int value_si = (1 << 30) + 1;
    signed long long int value_sll = (1ll << 62) + 1;

    unsigned char value_uc = (1u << 7) + 1;
    unsigned short value_us = (1u << 15) + 1;
    unsigned int value_ui = (1u << 31) + 1;
    unsigned long long int value_ull = (1ull << 63) + 1;

    gtl::big_unsigned big_unsigned_b(value_b);
    REQUIRE(big_unsigned_b == value_b);

    gtl::big_unsigned big_unsigned_c(value_c);
    REQUIRE(big_unsigned_c == value_c);

    gtl::big_unsigned big_unsigned_sc(value_sc);
    REQUIRE(big_unsigned_sc == value_sc);

    gtl::big_unsigned big_unsigned_ss(value_ss);
    REQUIRE(big_unsigned_ss == value_ss);

    gtl::big_unsigned big_unsigned_si(value_si);
    REQUIRE(big_unsigned_si == value_si);

    gtl::big_unsigned big_unsigned_sll(value_sll);
    REQUIRE(big_unsigned_sll == value_sll);

    gtl::big_unsigned big_unsigned_uc(value_uc);
    REQUIRE(big_unsigned_uc == value_uc);

    gtl::big_unsigned big_unsigned_us(value_us);
    REQUIRE(big_unsigned_us == value_us);

    gtl::big_unsigned big_unsigned_ui(value_ui);
    REQUIRE(big_unsigned_ui == value_ui);

    gtl::big_unsigned big_unsigned_ull(value_ull);
    REQUIRE(big_unsigned_ull == value_ull);
}

TEST(big_unsigned, constructor, string) {
    gtl::big_unsigned big_unsigned("123456", 6);
    REQUIRE(big_unsigned == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_unsigned big_unsigned_95(two_power_95, testbench::string_length(two_power_95));
    REQUIRE(check_power_of_two(big_unsigned_95, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
}

TEST(big_unsigned, constructor, bytes) {
    constexpr static const unsigned char data[6] = { 1, 2, 3, 4, 5, 6 };
    gtl::big_unsigned big_unsigned(&data[0], 6);
    REQUIRE(big_unsigned == 0x010203040506ull);

    constexpr static const unsigned char two_power_95[12] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    gtl::big_unsigned big_unsigned_95(&two_power_95[0], 12);
    REQUIRE(check_power_of_two(big_unsigned_95, 0, 95, 1));

    constexpr static const unsigned char two_power_333[42] = {
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    gtl::big_unsigned big_unsigned_333(&two_power_333[0], 42);
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
}

TEST(big_unsigned, constructor, copy) {
    gtl::big_unsigned big_unsigned("123456", 6);
    gtl::big_unsigned big_unsigned_copy(big_unsigned);
    REQUIRE(big_unsigned_copy == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_unsigned big_unsigned_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_unsigned big_unsigned_95_copy(big_unsigned_95);
    REQUIRE(check_power_of_two(big_unsigned_95_copy, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_333_copy(big_unsigned_333);
    REQUIRE(check_power_of_two(big_unsigned_333_copy, 0, 333, 1));
}

TEST(big_unsigned, constructor, move) {
    gtl::big_unsigned big_unsigned("123456", 6);
    gtl::big_unsigned big_unsigned_move(std::move(big_unsigned));
    REQUIRE(big_unsigned_move == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_unsigned big_unsigned_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_unsigned big_unsigned_95_move(std::move(big_unsigned_95));
    REQUIRE(check_power_of_two(big_unsigned_95_move, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_333_move(std::move(big_unsigned_333));
    REQUIRE(check_power_of_two(big_unsigned_333_move, 0, 333, 1));
}

TEST(big_unsigned, operator, copy) {
    gtl::big_unsigned big_unsigned("123456", 6);
    gtl::big_unsigned big_unsigned_copy = big_unsigned;
    REQUIRE(big_unsigned_copy == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_unsigned big_unsigned_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_unsigned big_unsigned_95_copy = big_unsigned_95;
    REQUIRE(check_power_of_two(big_unsigned_95_copy, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_333_copy = big_unsigned_333;
    REQUIRE(check_power_of_two(big_unsigned_333_copy, 0, 333, 1));
}

TEST(big_unsigned, operator, move) {
    gtl::big_unsigned big_unsigned("123456", 6);
    gtl::big_unsigned big_unsigned_move = std::move(big_unsigned);
    REQUIRE(big_unsigned_move == 123456);

    constexpr static const char* two_power_95 = "39614081257132168796771975168";
    gtl::big_unsigned big_unsigned_95(two_power_95, testbench::string_length(two_power_95));
    gtl::big_unsigned big_unsigned_95_move = std::move(big_unsigned_95);
    REQUIRE(check_power_of_two(big_unsigned_95_move, 0, 95, 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_333_move = std::move(big_unsigned_333);
    REQUIRE(check_power_of_two(big_unsigned_333_move, 0, 333, 1));
}

TEST(big_unsigned, operator, bool) {
    gtl::big_unsigned big_unsigned_true(true);
    gtl::big_unsigned big_unsigned_false(false);

    REQUIRE(big_unsigned_true);
    REQUIRE(!big_unsigned_false);
}

TEST(big_unsigned, operator, increment_pre) {
    gtl::big_unsigned big_unsigned(123);
    REQUIRE(big_unsigned == 123);
    REQUIRE(++big_unsigned == 124);
    REQUIRE(big_unsigned == 124);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
    REQUIRE(check_power_of_two(++big_unsigned_333, 1, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333, 1, 333, 1));
}

TEST(big_unsigned, operator, increment_post) {
    gtl::big_unsigned big_unsigned(123);
    REQUIRE(big_unsigned == 123);
    REQUIRE(big_unsigned++ == 123);
    REQUIRE(big_unsigned == 124);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333++, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333, 1, 333, 1));
}

TEST(big_unsigned, operator, decrement_pre) {
    gtl::big_unsigned big_unsigned(123);
    REQUIRE(big_unsigned == 123);
    REQUIRE(--big_unsigned == 122);
    REQUIRE(big_unsigned == 122);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
    REQUIRE(check_power_of_two(--big_unsigned_333, -1, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333, -1, 333, 1));
}

TEST(big_unsigned, operator, decrement_post) {
    gtl::big_unsigned big_unsigned(123);
    REQUIRE(big_unsigned == 123);
    REQUIRE(big_unsigned-- == 123);
    REQUIRE(big_unsigned == 122);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333--, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333, -1, 333, 1));
}

TEST(big_unsigned, operator, add) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs + big_unsigned_rhs == lhs + rhs);
    REQUIRE(big_unsigned_lhs + 1 == lhs + 1);
    REQUIRE(big_unsigned_rhs + 1 == rhs + 1);
    REQUIRE(gtl::big_unsigned(0) + big_unsigned_lhs == big_unsigned_lhs);
    REQUIRE(gtl::big_unsigned(0) + big_unsigned_rhs == big_unsigned_rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 + 1, 1, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 + 333, 333, 333, 1));
}

TEST(big_unsigned, operator, subtract) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs - big_unsigned_rhs == lhs - rhs);
    REQUIRE(big_unsigned_lhs - 1 == lhs - 1);
    REQUIRE(big_unsigned_rhs - 1 == rhs - 1);
    REQUIRE(big_unsigned_lhs - 0 == big_unsigned_lhs);
    REQUIRE(big_unsigned_rhs - 0 == big_unsigned_rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 - 1, -1, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 - 333, -333, 333, 1));
}

TEST(big_unsigned, operator, multiply) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs * big_unsigned_rhs == lhs * rhs);
    REQUIRE(big_unsigned_lhs * 1 == lhs * 1);
    REQUIRE(big_unsigned_rhs * 1 == rhs * 1);
    REQUIRE(gtl::big_unsigned(0) * big_unsigned_lhs == 0);
    REQUIRE(gtl::big_unsigned(0) * big_unsigned_rhs == 0);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 * 1, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 * 333, 0, 333, 333));
}

TEST(big_unsigned, operator, divide) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs / big_unsigned_rhs == lhs / rhs);
    REQUIRE(big_unsigned_lhs / 1 == lhs / 1);
    REQUIRE(big_unsigned_rhs / 1 == rhs / 1);
    REQUIRE(gtl::big_unsigned(0) / big_unsigned_lhs == 0);
    REQUIRE(gtl::big_unsigned(0) / big_unsigned_rhs == 0);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 / 1, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 / 1024, 0, 333 - 10, 1));
}

TEST(big_unsigned, operator, modulo) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs % big_unsigned_rhs == lhs % rhs);
    REQUIRE(big_unsigned_lhs % 1 == lhs % 1);
    REQUIRE(big_unsigned_rhs % 1 == rhs % 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 % 1, 0, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 2, 0, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 3, 2, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 4, 0, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 5, 2, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 6, 2, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 7, 1, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 8, 0, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % 9, 8, 0, 0));
    REQUIRE(check_power_of_two(big_unsigned_333 % (big_unsigned_333 + 1), 0, 333, 1));
}

TEST(big_unsigned, operator, bit_shift_left) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs << 1 == lhs << 1);
    REQUIRE(big_unsigned_rhs << 1 == rhs << 1);
    REQUIRE(big_unsigned_lhs << 0 == lhs);
    REQUIRE(big_unsigned_rhs << 0 == rhs);
    REQUIRE(big_unsigned_rhs << 61 == lhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 << 0, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 << 1, 0, 334, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 << 2, 0, 335, 1));
}

TEST(big_unsigned, operator, bit_shift_right) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs >> 1 == lhs >> 1);
    REQUIRE(big_unsigned_rhs >> 1 == rhs >> 1);
    REQUIRE(big_unsigned_lhs >> 0 == lhs >> 0);
    REQUIRE(big_unsigned_rhs >> 0 == rhs >> 0);
    REQUIRE(big_unsigned_lhs >> 61 == rhs);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(check_power_of_two(big_unsigned_333 >> 0, 0, 333, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 >> 1, 0, 332, 1));
    REQUIRE(check_power_of_two(big_unsigned_333 >> 2, 0, 331, 1));
}

TEST(big_unsigned, operator, bit_and) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs & big_unsigned_rhs) == 0);
    REQUIRE((big_unsigned_lhs & big_unsigned_lhs) == big_unsigned_lhs);
    REQUIRE((big_unsigned_rhs & big_unsigned_rhs) == big_unsigned_rhs);
    REQUIRE((big_unsigned_lhs & 2) == 0);
    REQUIRE((big_unsigned_rhs & 2) == 2);
    REQUIRE((big_unsigned_lhs & 1) == 0);
    REQUIRE((big_unsigned_rhs & 1) == 0);
    REQUIRE((big_unsigned_lhs & 0) == 0);
    REQUIRE((big_unsigned_rhs & 0) == 0);
    REQUIRE(((big_unsigned_lhs - 1) & big_unsigned_lhs) == 0);
    REQUIRE(((big_unsigned_lhs - 1) & big_unsigned_rhs) == rhs);
    REQUIRE(((big_unsigned_lhs - 1) & 1) == 1);
    REQUIRE(((big_unsigned_rhs - 1) & 1) == 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_333 & big_unsigned_333) == big_unsigned_333);
    REQUIRE((big_unsigned_333 & 1) == 0);
    REQUIRE((big_unsigned_333 & 0) == 0);
    REQUIRE(((big_unsigned_333 - 1) & big_unsigned_333) == 0);
    REQUIRE(((big_unsigned_333 - 1) & big_unsigned_lhs) == lhs);
    REQUIRE(((big_unsigned_333 - 1) & big_unsigned_rhs) == rhs);
    REQUIRE(((big_unsigned_333 - 1) & 1) == 1);
}

TEST(big_unsigned, operator, bit_or) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs | big_unsigned_rhs) == (lhs | rhs));
    REQUIRE((big_unsigned_lhs | big_unsigned_lhs) == big_unsigned_lhs);
    REQUIRE((big_unsigned_rhs | big_unsigned_rhs) == big_unsigned_rhs);
    REQUIRE((big_unsigned_lhs | 1) == (lhs | 1));
    REQUIRE((big_unsigned_rhs | 1) == (rhs | 1));
    REQUIRE((big_unsigned_lhs | 0) == lhs);
    REQUIRE((big_unsigned_rhs | 0) == rhs);
    REQUIRE(((big_unsigned_lhs - 1) | big_unsigned_lhs) == ((lhs - 1) | lhs));
    REQUIRE(((big_unsigned_lhs - 1) | big_unsigned_rhs) == ((lhs - 1) | rhs));
    REQUIRE(((big_unsigned_lhs - 1) | 1) == ((lhs - 1) | 1));
    REQUIRE(((big_unsigned_rhs - 1) | 1) == ((rhs - 1) | 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_333 | big_unsigned_333) == big_unsigned_333);
    REQUIRE((big_unsigned_333 | 1) == (gtl::big_unsigned(1) | big_unsigned_333));
    REQUIRE((big_unsigned_333 | 0) == big_unsigned_333);
    REQUIRE(((big_unsigned_333 - 1) | big_unsigned_333) == (big_unsigned_333 | (big_unsigned_333 - 1)));
    REQUIRE(((big_unsigned_333 - 1) | big_unsigned_lhs) == (big_unsigned_333 - 1));
    REQUIRE(((big_unsigned_333 - 1) | big_unsigned_rhs) == (big_unsigned_333 - 1));
    REQUIRE(((big_unsigned_333 - 1) | 1) == (big_unsigned_333 - 1));
}

TEST(big_unsigned, operator, bit_xor) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs ^ big_unsigned_rhs) == (lhs ^ rhs));
    REQUIRE((big_unsigned_lhs ^ big_unsigned_lhs) == 0);
    REQUIRE((big_unsigned_rhs ^ big_unsigned_rhs) == 0);
    REQUIRE((big_unsigned_lhs ^ 1) == (lhs ^ 1));
    REQUIRE((big_unsigned_rhs ^ 1) == (rhs ^ 1));
    REQUIRE((big_unsigned_lhs ^ 0) == lhs);
    REQUIRE((big_unsigned_rhs ^ 0) == rhs);
    REQUIRE(((big_unsigned_lhs - 1) ^ big_unsigned_lhs) == ((lhs - 1) ^ lhs));
    REQUIRE(((big_unsigned_lhs - 1) ^ big_unsigned_rhs) == ((lhs - 1) ^ rhs));
    REQUIRE(((big_unsigned_lhs - 1) ^ 1) == ((lhs - 1) ^ 1));
    REQUIRE(((big_unsigned_rhs - 1) ^ 1) == ((rhs - 1) ^ 1));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_333 ^ big_unsigned_333) == 0);
    REQUIRE((big_unsigned_333 ^ 1) == (gtl::big_unsigned(1) ^ big_unsigned_333));
    REQUIRE((big_unsigned_333 ^ 0) == big_unsigned_333);
    REQUIRE(((big_unsigned_333 - 1) ^ big_unsigned_333) == (big_unsigned_333 ^ (big_unsigned_333 - 1)));
    REQUIRE(((big_unsigned_333 - 1) ^ big_unsigned_lhs) == (big_unsigned_lhs ^ (big_unsigned_333 - 1)));
    REQUIRE(((big_unsigned_333 - 1) ^ big_unsigned_rhs) == (big_unsigned_rhs ^ (big_unsigned_333 - 1)));
    REQUIRE(((big_unsigned_333 - 1) ^ 1) == (gtl::big_unsigned(1) ^ (big_unsigned_333 - 1)));
}

TEST(big_unsigned, operator, bit_not) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(~big_unsigned_lhs == ~lhs);
    REQUIRE(~big_unsigned_rhs == ~static_cast<unsigned int>(rhs));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    constexpr static const char* two_power_352 = "9173994463960286046443283581208347763186259956673124494950355357547691504353939232280074212440502746218496";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_352(two_power_352, testbench::string_length(two_power_352));
    REQUIRE(~big_unsigned_333 == ((big_unsigned_352 - 1) ^ big_unsigned_333));
    REQUIRE(~(big_unsigned_352 - 1) == 0);
}

TEST(big_unsigned, operator, bit_and_not) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_lhs, big_unsigned_rhs) == lhs);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_rhs, big_unsigned_lhs) == rhs);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_lhs, big_unsigned_lhs) == 0);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_rhs, big_unsigned_rhs) == 0);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    constexpr static const char* two_power_352 = "9173994463960286046443283581208347763186259956673124494950355357547691504353939232280074212440502746218496";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    gtl::big_unsigned big_unsigned_352(two_power_352, testbench::string_length(two_power_352));
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_333, big_unsigned_352) == big_unsigned_333);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_352, big_unsigned_333) == big_unsigned_352);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_333, big_unsigned_333) == 0);
    REQUIRE(gtl::big_unsigned::bit_and_not(big_unsigned_352, big_unsigned_352) == 0);
}

TEST(big_unsigned, operator, less_than) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs < big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_rhs < big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_lhs < big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_rhs < big_unsigned_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs < big_unsigned_333) == true);
    REQUIRE((big_unsigned_rhs < big_unsigned_333) == true);
    REQUIRE((big_unsigned_333 < big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_333 < big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_333 < big_unsigned_333) == false);
}

TEST(big_unsigned, operator, greater_than) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs > big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_rhs > big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_lhs > big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_rhs > big_unsigned_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs > big_unsigned_333) == false);
    REQUIRE((big_unsigned_rhs > big_unsigned_333) == false);
    REQUIRE((big_unsigned_333 > big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_333 > big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_333 > big_unsigned_333) == false);
}

TEST(big_unsigned, operator, less_than_or_equal_to) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs <= big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_rhs <= big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_lhs <= big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_rhs <= big_unsigned_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs <= big_unsigned_333) == true);
    REQUIRE((big_unsigned_rhs <= big_unsigned_333) == true);
    REQUIRE((big_unsigned_333 <= big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_333 <= big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_333 <= big_unsigned_333) == true);
}

TEST(big_unsigned, operator, greater_than_or_equal_to) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs >= big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_rhs >= big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_lhs >= big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_rhs >= big_unsigned_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs >= big_unsigned_333) == false);
    REQUIRE((big_unsigned_rhs >= big_unsigned_333) == false);
    REQUIRE((big_unsigned_333 >= big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_333 >= big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_333 >= big_unsigned_333) == true);
}

TEST(big_unsigned, operator, equal_to) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs == big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_rhs == big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_lhs == big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_rhs == big_unsigned_rhs) == true);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs == big_unsigned_333) == false);
    REQUIRE((big_unsigned_rhs == big_unsigned_333) == false);
    REQUIRE((big_unsigned_333 == big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_333 == big_unsigned_rhs) == false);
    REQUIRE((big_unsigned_333 == big_unsigned_333) == true);
}

TEST(big_unsigned, operator, not_equal_to) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE((big_unsigned_lhs != big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_rhs != big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_lhs != big_unsigned_lhs) == false);
    REQUIRE((big_unsigned_rhs != big_unsigned_rhs) == false);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE((big_unsigned_lhs != big_unsigned_333) == true);
    REQUIRE((big_unsigned_rhs != big_unsigned_333) == true);
    REQUIRE((big_unsigned_333 != big_unsigned_lhs) == true);
    REQUIRE((big_unsigned_333 != big_unsigned_rhs) == true);
    REQUIRE((big_unsigned_333 != big_unsigned_333) == false);
}

TEST(big_unsigned, function, get_length_bits) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.get_length_bits() == 63);
    REQUIRE(big_unsigned_rhs.get_length_bits() == 2);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_unsigned_333.get_length_bits() == 334);
}

TEST(big_unsigned, function, get_length_bytes) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.get_length_bytes() == 8);
    REQUIRE(big_unsigned_rhs.get_length_bytes() == 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_unsigned_333.get_length_bytes() == 42);
}

TEST(big_unsigned, function, get_length_decimal) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.get_length_decimal() == 19);
    REQUIRE(big_unsigned_rhs.get_length_decimal() == 1);

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_unsigned_333.get_length_decimal() == 101);
}

TEST(big_unsigned, function, to_string) {
    unsigned long long int lhs = 1ull << 62;
    unsigned long long int rhs = 2;

    char string_lhs[20];
    char string_rhs[2];
    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.to_string(string_lhs, 20) == 19);
    REQUIRE(big_unsigned_rhs.to_string(string_rhs, 2) == 1);
    REQUIRE(testbench::is_string_same(string_lhs, "4611686018427387904"));
    REQUIRE(testbench::is_string_same(string_rhs, "2"));

    constexpr static const char* two_power_333 = "17498005798264095394980017816940970922825355447145699491406164851279623993595007385788105416184430592";
    char string_333[102];
    gtl::big_unsigned big_unsigned_333(two_power_333, testbench::string_length(two_power_333));
    REQUIRE(big_unsigned_333.to_string(string_333, 102) == 101);
    REQUIRE(testbench::is_string_same(string_333, two_power_333));
}

TEST(big_unsigned, function, to_bytes) {
    const unsigned long long int lhs = 1ull << 62;
    const unsigned long long int rhs = 2;

    constexpr static const auto swap_endian = [](unsigned long long value) -> unsigned long long {
        return (value >> 56) |
               ((value << 40) & 0x00FF000000000000ull) |
               ((value << 24) & 0x0000FF0000000000ull) |
               ((value << 8) & 0x000000FF00000000ull) |
               ((value >> 8) & 0x00000000FF000000ull) |
               ((value >> 24) & 0x0000000000FF0000ull) |
               ((value >> 40) & 0x000000000000FF00ull) |
               (value << 56);
    };

    unsigned char bytes_lhs[8];
    unsigned char bytes_rhs[1];
    unsigned char bytes_rhs_padded[8];
    const unsigned long long int swapped_lhs = swap_endian(lhs);
    const unsigned long long int swapped_rhs = swap_endian(rhs);
    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.to_bytes(&bytes_lhs[0], 8) == 8);
    REQUIRE(big_unsigned_rhs.to_bytes(&bytes_rhs[0], 1) == 1);
    REQUIRE(big_unsigned_rhs.to_bytes(&bytes_rhs_padded[0], 8, true) == 1);
    REQUIRE(testbench::is_memory_same(&bytes_lhs[0], &swapped_lhs, 8));
    REQUIRE(testbench::is_memory_same(&bytes_rhs[0], &rhs, 1));
    REQUIRE(testbench::is_memory_same(&bytes_rhs_padded[0], &swapped_rhs, 8));

    constexpr static const unsigned char two_power_333[42] = {
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    unsigned char bytes_333[42];
    gtl::big_unsigned big_unsigned_333(two_power_333, 42);
    REQUIRE(big_unsigned_333.to_bytes(&bytes_333[0], 42) == 42);
    REQUIRE(testbench::is_memory_same(&bytes_333[0], &two_power_333[0], 42));
}

TEST(big_unsigned, function, to_unsigned_integer) {
    unsigned int lhs = 1ull << 31;
    unsigned int rhs = 2;
    gtl::big_unsigned big_unsigned_lhs(lhs);
    gtl::big_unsigned big_unsigned_rhs(rhs);
    REQUIRE(big_unsigned_lhs.to_unsigned_integer() == lhs);
    REQUIRE(big_unsigned_rhs.to_unsigned_integer() == rhs);
}
