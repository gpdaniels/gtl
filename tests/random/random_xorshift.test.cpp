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

#include <random/random_xorshift>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(random_xorshift, traits, standard) {
    REQUIRE(std::is_pod<gtl::random_xorshift>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::random_xorshift>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::random_xorshift>::value == true, "Expected std::is_trivially_copyable to be true.");

    REQUIRE(std::is_standard_layout<gtl::random_xorshift>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(random_xorshift, constructor, empty) {
    gtl::random_xorshift random_xorshift;
    testbench::do_not_optimise_away(random_xorshift);
}

TEST(random_xorshift, constructor, seed) {
    gtl::random_xorshift random_xorshift(0x01234567);
    testbench::do_not_optimise_away(random_xorshift);
}

TEST(random_xorshift, function, seed) {
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
}

TEST(random_xorshift, function, get_random_raw) {
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    unsigned int random_raw = random_xorshift.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 514137109u), "Pseudo-random number 1 was %u, expected %u", random_raw, 514137109u);
    random_raw = random_xorshift.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 2827593414u), "Pseudo-random number 2 was %u, expected %u", random_raw, 2827593414u);
    random_raw = random_xorshift.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 416909229u), "Pseudo-random number 3 was %u, expected %u", random_raw, 416909229u);
    random_raw = random_xorshift.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1502090661u), "Pseudo-random number 4 was %u, expected %u", random_raw, 1502090661u);
    random_raw = random_xorshift.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 367073627u), "Pseudo-random number 5 was %u, expected %u", random_raw, 367073627u);
}

TEST(random_xorshift, function, get_random_exclusive) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    double random_exclusive = round(random_xorshift.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.119707), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.119707);
    random_exclusive = round(random_xorshift.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.658350), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.658350);
    random_exclusive = round(random_xorshift.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.097069), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.097069);
    random_exclusive = round(random_xorshift.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.349733), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.349733);
    random_exclusive = round(random_xorshift.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.085466), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.085466);
}

TEST(random_xorshift, function, get_random_exclusive_top) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    double random_exclusive_top = round(random_xorshift.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.119707), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.119707);
    random_exclusive_top = round(random_xorshift.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.658350), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.658350);
    random_exclusive_top = round(random_xorshift.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.097069), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.097069);
    random_exclusive_top = round(random_xorshift.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.349733), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.349733);
    random_exclusive_top = round(random_xorshift.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.085466), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.085466);
}

TEST(random_xorshift, function, get_random_inclusive) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    double random_inclusive = round(random_xorshift.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.119707), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.119707);
    random_inclusive = round(random_xorshift.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.658350), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.658350);
    random_inclusive = round(random_xorshift.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.097069), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.097069);
    random_inclusive = round(random_xorshift.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.349733), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.349733);
    random_inclusive = round(random_xorshift.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.085466), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.085466);
}

TEST(random_xorshift, function, get_random_bounded_long) {
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    unsigned int random = random_xorshift.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 1 was %u, expected %u", random, 1u);
    random = random_xorshift.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 2 was %u, expected %u", random, 0u);
    random = random_xorshift.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 3 was %u, expected %u", random, 1u);
    random = random_xorshift.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 4 was %u, expected %u", random, 1u);
    random = random_xorshift.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 5 was %u, expected %u", random, 1u);
}

TEST(random_xorshift, function, get_random_bounded_double) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    double random = round(random_xorshift.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.880293), "Pseudo-random number 1 was %lf, expected %lf", random, -0.880293);
    random = round(random_xorshift.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.341650), "Pseudo-random number 2 was %lf, expected %lf", random, -0.341650);
    random = round(random_xorshift.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.902931), "Pseudo-random number 3 was %lf, expected %lf", random, -0.902931);
    random = round(random_xorshift.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.650267), "Pseudo-random number 4 was %lf, expected %lf", random, -0.650267);
    random = round(random_xorshift.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.914534), "Pseudo-random number 5 was %lf, expected %lf", random, -0.914534);
}

TEST(random_xorshift, evaluation, random_numbers) {
    gtl::random_xorshift random_xorshift;
    random_xorshift.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_xorshift.get_random(0u, 1u) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, 736), "Pseudo-random number generator bias was %d, expected %d", random_bias, 736);
}
