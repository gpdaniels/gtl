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

#include <random/random_romu_64>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(random_romu_64, traits, standard) {
    REQUIRE((std::is_pod<gtl::random_romu_64>::value == false));

    REQUIRE((std::is_trivial<gtl::random_romu_64>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::random_romu_64>::value == true));

    REQUIRE((std::is_standard_layout<gtl::random_romu_64>::value == true));
}

TEST(random_romu_64, constructor, empty) {
    gtl::random_romu_64 random_romu_64;
    testbench::do_not_optimise_away(random_romu_64);
}

TEST(random_romu_64, constructor, seed) {
    gtl::random_romu_64 random_romu_64(0x01234567);
    testbench::do_not_optimise_away(random_romu_64);
}

TEST(random_romu_64, function, seed) {
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
}

TEST(random_romu_64, function, get_random_raw) {
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    unsigned long long int random_raw = random_romu_64.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 3689348814722856532ull), "Pseudo-random number 1 was %llu, expected %llu", random_raw, 3689348814722856532ull);
    random_raw = random_romu_64.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 298458395553402105ull), "Pseudo-random number 2 was %llu, expected %llu", random_raw, 298458395553402105ull);
    random_raw = random_romu_64.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 15827028583747069971ull), "Pseudo-random number 3 was %llu, expected %llu", random_raw, 15827028583747069971ull);
    random_raw = random_romu_64.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 11288605026576837290ull), "Pseudo-random number 4 was %llu, expected %llu", random_raw, 11288605026576837290ull);
    random_raw = random_romu_64.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 13982910176031586462ull), "Pseudo-random number 5 was %llu, expected %llu", random_raw, 13982910176031586462ull);
}

TEST(random_romu_64, function, get_random_exclusive) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    double random_exclusive = round(random_romu_64.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.200000), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.200000);
    random_exclusive = round(random_romu_64.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.016179), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.016179);
    random_exclusive = round(random_romu_64.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.857985), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.857985);
    random_exclusive = round(random_romu_64.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.611957), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.611957);
    random_exclusive = round(random_romu_64.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.758015), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.758015);
}

TEST(random_romu_64, function, get_random_exclusive_top) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    double random_exclusive_top = round(random_romu_64.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.200000), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.200000);
    random_exclusive_top = round(random_romu_64.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.016179), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.016179);
    random_exclusive_top = round(random_romu_64.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.857985), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.857985);
    random_exclusive_top = round(random_romu_64.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.611957), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.611957);
    random_exclusive_top = round(random_romu_64.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.758015), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.758015);
}

TEST(random_romu_64, function, get_random_inclusive) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    double random_inclusive = round(random_romu_64.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.200000), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.200000);
    random_inclusive = round(random_romu_64.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.016179), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.016179);
    random_inclusive = round(random_romu_64.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.857985), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.857985);
    random_inclusive = round(random_romu_64.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.611957), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.611957);
    random_inclusive = round(random_romu_64.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.758015), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.758015);
}

TEST(random_romu_64, function, get_random_bounded_long) {
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    unsigned long long int random = random_romu_64.get_random(0ull, 1ull);
    REQUIRE(testbench::is_value_equal(random, 0ull), "Pseudo-random number 1 was %llu, expected %llu", random, 0ull);
    random = random_romu_64.get_random(0ull, 1ull);
    REQUIRE(testbench::is_value_equal(random, 1ull), "Pseudo-random number 2 was %llu, expected %llu", random, 1ull);
    random = random_romu_64.get_random(0ull, 1ull);
    REQUIRE(testbench::is_value_equal(random, 1ull), "Pseudo-random number 3 was %llu, expected %llu", random, 1ull);
    random = random_romu_64.get_random(0ull, 1ull);
    REQUIRE(testbench::is_value_equal(random, 0ull), "Pseudo-random number 4 was %llu, expected %llu", random, 0ull);
    random = random_romu_64.get_random(0ull, 1ull);
    REQUIRE(testbench::is_value_equal(random, 0ull), "Pseudo-random number 5 was %llu, expected %llu", random, 0ull);
}

TEST(random_romu_64, function, get_random_bounded_double) {
    auto round = [](double value) -> double {
        return std::round(value * 1000000.0) / 1000000.0;
    };
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    double random = round(random_romu_64.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.800000), "Pseudo-random number 1 was %lf, expected %lf", random, -0.800000);
    random = round(random_romu_64.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.983821), "Pseudo-random number 2 was %lf, expected %lf", random, -0.983821);
    random = round(random_romu_64.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.142015), "Pseudo-random number 3 was %lf, expected %lf", random, -0.142015);
    random = round(random_romu_64.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.388043), "Pseudo-random number 4 was %lf, expected %lf", random, -0.388043);
    random = round(random_romu_64.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.241985), "Pseudo-random number 5 was %lf, expected %lf", random, -0.241985);
}

TEST(random_romu_64, evaluation, random_numbers) {
    gtl::random_romu_64 random_romu_64;
    random_romu_64.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_romu_64.get_random(0ull, 1ull) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, -52), "Pseudo-random number generator bias was %d, expected %d", random_bias, -52);
}
