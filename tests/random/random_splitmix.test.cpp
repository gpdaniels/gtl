/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include <random/random_splitmix>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(random_splitmix, traits, standard) {
    REQUIRE(std::is_pod<gtl::random_splitmix>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::random_splitmix>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::random_splitmix>::value == true, "Expected std::is_trivially_copyable to be true.");

    REQUIRE(std::is_standard_layout<gtl::random_splitmix>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(random_splitmix, constructor, empty) {
    gtl::random_splitmix random_splitmix;
    testbench::do_not_optimise_away(random_splitmix);
}

TEST(random_splitmix, constructor, seed) {
    gtl::random_splitmix random_splitmix(0x01234567);
    testbench::do_not_optimise_away(random_splitmix);
}

TEST(random_splitmix, function, seed) {
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
}

TEST(random_splitmix, function, get_random_raw) {
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    unsigned int random_raw = random_splitmix.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1953078471u), "Pseudo-random number 1 was %u, expected %u", random_raw, 1953078471u);
    random_raw = random_splitmix.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 2159333104u), "Pseudo-random number 2 was %u, expected %u", random_raw, 2159333104u);
    random_raw = random_splitmix.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1013292015u), "Pseudo-random number 3 was %u, expected %u", random_raw, 1013292015u);
    random_raw = random_splitmix.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 1324138226u), "Pseudo-random number 4 was %u, expected %u", random_raw, 1324138226u);
    random_raw = random_splitmix.get_random_raw();
    REQUIRE(testbench::is_value_equal(random_raw, 2004240380u), "Pseudo-random number 5 was %u, expected %u", random_raw, 2004240380u);
}

TEST(random_splitmix, function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    double random_exclusive = round(random_splitmix.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.454737), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.454737);
    random_exclusive = round(random_splitmix.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.502759), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.502759);
    random_exclusive = round(random_splitmix.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.235925), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.235925);
    random_exclusive = round(random_splitmix.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.308300), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.308300);
    random_exclusive = round(random_splitmix.get_random_exclusive());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.466649), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.466649);
}

TEST(random_splitmix, function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    double random_exclusive_top = round(random_splitmix.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.454737), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.454737);
    random_exclusive_top = round(random_splitmix.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.502759), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.502759);
    random_exclusive_top = round(random_splitmix.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.235925), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.235925);
    random_exclusive_top = round(random_splitmix.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.308300), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.308300);
    random_exclusive_top = round(random_splitmix.get_random_exclusive_top());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.466649), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.466649);
}

TEST(random_splitmix, function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    double random_inclusive = round(random_splitmix.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.454737), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.454737);
    random_inclusive = round(random_splitmix.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.502759), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.502759);
    random_inclusive = round(random_splitmix.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.235925), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.235925);
    random_inclusive = round(random_splitmix.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.308300), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.308300);
    random_inclusive = round(random_splitmix.get_random_inclusive());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.466649), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.466649);
}

TEST(random_splitmix, function, get_random_bounded_long) {
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    unsigned int random = random_splitmix.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 1 was %u, expected %u", random, 1u);
    random = random_splitmix.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 2 was %u, expected %u", random, 0u);
    random = random_splitmix.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 3 was %u, expected %u", random, 1u);
    random = random_splitmix.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 4 was %u, expected %u", random, 0u);
    random = random_splitmix.get_random(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 5 was %u, expected %u", random, 0u);
}

TEST(random_splitmix, function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    double random = round(random_splitmix.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.545263), "Pseudo-random number 1 was %lf, expected %lf", random, -0.545263);
    random = round(random_splitmix.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.497241), "Pseudo-random number 2 was %lf, expected %lf", random, -0.497241);
    random = round(random_splitmix.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.764075), "Pseudo-random number 3 was %lf, expected %lf", random, -0.764075);
    random = round(random_splitmix.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.691700), "Pseudo-random number 4 was %lf, expected %lf", random, -0.691700);
    random = round(random_splitmix.get_random(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.533351), "Pseudo-random number 5 was %lf, expected %lf", random, -0.533351);
}

TEST(random_splitmix, evaluation, random_numbers) {
    gtl::random_splitmix random_splitmix;
    random_splitmix.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_splitmix.get_random(0u, 1u) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, -166), "Pseudo-random number generator bias was %d, expected %d", random_bias, -166);
}
