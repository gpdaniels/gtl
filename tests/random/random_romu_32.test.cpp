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
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <random/random_romu_32>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(random_romu_32, traits, standard) {
    REQUIRE((std::is_pod<gtl::random_romu_32>::value == false));

    REQUIRE((std::is_trivial<gtl::random_romu_32>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::random_romu_32>::value == true));

    REQUIRE((std::is_standard_layout<gtl::random_romu_32>::value == true));
}

TEST(random_romu_32, constructor, empty) {
    gtl::random_romu_32 random_romu_32;
    testbench::do_not_optimise_away(random_romu_32);
}

TEST(random_romu_32, constructor, seed) {
    gtl::random_romu_32 random_romu_32(0x01234567);
    testbench::do_not_optimise_away(random_romu_32);
}

TEST(random_romu_32, constructor, seed_multi) {
    gtl::random_romu_32 random_romu_32(0x01234567, 0x01234567, 0x01234567, 0x0123456);
    testbench::do_not_optimise_away(random_romu_32);
}

TEST(random_romu_32, function, seed) {
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    random_romu_32.seed(0x01234567, 0x01234567, 0x01234567, 0x0123456);
}

TEST(random_romu_32, function, get_random_raw) {
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    unsigned int random_raw = random_romu_32.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 839939668u), "Pseudo-random number 1 was %u, expected %u", random_raw, 839939668u);
    random_raw = random_romu_32.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 3408797144u), "Pseudo-random number 2 was %u, expected %u", random_raw, 3408797144u);
    random_raw = random_romu_32.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1715344423u), "Pseudo-random number 3 was %u, expected %u", random_raw, 1715344423u);
    random_raw = random_romu_32.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 109390076u), "Pseudo-random number 4 was %u, expected %u", random_raw, 109390076u);
    random_raw = random_romu_32.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 619426106u), "Pseudo-random number 5 was %u, expected %u", random_raw, 619426106u);
}

TEST(random_romu_32, function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    double random_exclusive = round(random_romu_32.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.195564), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.195564);
    random_exclusive = round(random_romu_32.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.793672), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.793672);
    random_exclusive = round(random_romu_32.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.399385), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.399385);
    random_exclusive = round(random_romu_32.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.025469), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.025469);
    random_exclusive = round(random_romu_32.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.144221), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.144221);
}

TEST(random_romu_32, function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    double random_exclusive_top = round(random_romu_32.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.195564), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.195564);
    random_exclusive_top = round(random_romu_32.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.793672), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.793672);
    random_exclusive_top = round(random_romu_32.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.399385), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.399385);
    random_exclusive_top = round(random_romu_32.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.025469), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.025469);
    random_exclusive_top = round(random_romu_32.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.144221), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.144221);
}

TEST(random_romu_32, function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    double random_inclusive = round(random_romu_32.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.195564), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.195564);
    random_inclusive = round(random_romu_32.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.793672), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.793672);
    random_inclusive = round(random_romu_32.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.399385), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.399385);
    random_inclusive = round(random_romu_32.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.025469), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.025469);
    random_inclusive = round(random_romu_32.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.144221), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.144221);
}

TEST(random_romu_32, function, get_random_bounded_long) {
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    unsigned int random = random_romu_32.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 1 was %u, expected %u", random, 0u);
    random = random_romu_32.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 2 was %u, expected %u", random, 0u);
    random = random_romu_32.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 3 was %u, expected %u", random, 1u);
    random = random_romu_32.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 4 was %u, expected %u", random, 0u);
    random = random_romu_32.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 5 was %u, expected %u", random, 0u);
}

TEST(random_romu_32, function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    double random = round(random_romu_32.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.804436), "Pseudo-random number 1 was %lf, expected %lf", random, -0.804436);
    random = round(random_romu_32.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.206328), "Pseudo-random number 2 was %lf, expected %lf", random, -0.206328);
    random = round(random_romu_32.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.600615), "Pseudo-random number 3 was %lf, expected %lf", random, -0.600615);
    random = round(random_romu_32.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.974531), "Pseudo-random number 4 was %lf, expected %lf", random, -0.974531);
    random = round(random_romu_32.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.855779), "Pseudo-random number 5 was %lf, expected %lf", random, -0.855779);
}

TEST(random_romu_32, evaluation, random_numbers) {
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_romu_32.get_random(0u, 1u) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, -1388), "Pseudo-random number generator bias was %d, expected %d", random_bias, -1388);
}
