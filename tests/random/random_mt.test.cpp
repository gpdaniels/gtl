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
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <random/random_mt>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(random_mt, traits, standard) {
    REQUIRE(std::is_pod<gtl::random_mt>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::random_mt>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::random_mt>::value == true, "Expected std::is_trivially_copyable to be true.");

    REQUIRE(std::is_standard_layout<gtl::random_mt>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(random_mt, constructor, empty) {
    gtl::random_mt random_mt;
    testbench::do_not_optimise_away(random_mt);
}

TEST(random_mt, constructor, seed_value) {
    gtl::random_mt random_mt(0x01234567);
    testbench::do_not_optimise_away(random_mt);
}

TEST(random_mt, constructor, seed_array) {
    unsigned int seed_values[5] = { 0x01234567, 0x12345678, 0x23456789, 0x34567890, 0x45678901 };
    gtl::random_mt random_mt(&seed_values[0], 5);
    testbench::do_not_optimise_away(random_mt);
}

TEST(random_mt, function, seed_value) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
}

TEST(random_mt, function, seed_array) {
    unsigned int seed_values[5] = { 0x01234567, 0x12345678, 0x23456789, 0x34567890, 0x45678901 };
    gtl::random_mt random_mt;
    random_mt.seed(&seed_values[0], 5);
}

TEST(random_mt, function, get_random_raw) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    unsigned int random_raw = random_mt.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1802874368u), "Pseudo-random number 1 was %u, expected %u", random_raw, 1802874368u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 453914126u), "Pseudo-random number 2 was %u, expected %u", random_raw, 453914126u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 2896676756u), "Pseudo-random number 3 was %u, expected %u", random_raw, 2896676756u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1924626451u), "Pseudo-random number 4 was %u, expected %u", random_raw, 1924626451u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1073351711u), "Pseudo-random number 5 was %u, expected %u", random_raw, 1073351711u);
}

TEST(random_mt, function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.419764), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.419764);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.105685), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.105685);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.674435), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.674435);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.448112), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.448112);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.249909), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.249909);
}

TEST(random_mt, function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.419764), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.419764);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.105685), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.105685);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.674435), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.674435);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.448112), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.448112);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.249909), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.249909);
}

TEST(random_mt, function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.419764), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.419764);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.105685), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.105685);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.674435), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.674435);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.448112), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.448112);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.249909), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.249909);
}

TEST(random_mt, function, get_random_bounded_long) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    unsigned int random = random_mt.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 1 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 2 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 3 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 4 was %u, expected %u", random, 1u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 5 was %u, expected %u", random, 1u);
}

TEST(random_mt, function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.580236), "Pseudo-random number 1 was %lf, expected %lf", random, -0.580236);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.894315), "Pseudo-random number 2 was %lf, expected %lf", random, -0.894315);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.325565), "Pseudo-random number 3 was %lf, expected %lf", random, -0.325565);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.551888), "Pseudo-random number 4 was %lf, expected %lf", random, -0.551888);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.750091), "Pseudo-random number 5 was %lf, expected %lf", random, -0.750091);
}

TEST(random_mt, evaluation, random_numbers) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_mt.get_random(0u, 1u) == 0u) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, 74), "Pseudo-random number generator bias was %d, expected %d", random_bias, 74);
}
