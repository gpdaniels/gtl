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
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <random/random_rlcg>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(random_rlcg, constructor, empty) {
    gtl::random_rlcg random_rlcg;
    testbench::do_not_optimise_away(random_rlcg);
}

TEST(random_rlcg, constructor, seed) {
    gtl::random_rlcg random_rlcg(0x01234567);
    testbench::do_not_optimise_away(random_rlcg);
}

TEST(random_rlcg, function, seed) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
}

TEST(random_rlcg, function, get_random_raw) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    unsigned int random_raw = random_rlcg.get_random_raw_forward();
    REQUIRE(testbench::is_value_equal(random_raw, 949768384u), "Pseudo-random number 1 was %u, expected %u", random_raw, 949768384u);
    random_raw = random_rlcg.get_random_raw_forward();
    REQUIRE(testbench::is_value_equal(random_raw, 2693729295u), "Pseudo-random number 2 was %u, expected %u", random_raw, 2693729295u);
    random_raw = random_rlcg.get_random_raw_forward();
    REQUIRE(testbench::is_value_equal(random_raw, 167867113u), "Pseudo-random number 3 was %u, expected %u", random_raw, 167867113u);
    random_raw = random_rlcg.get_random_raw_forward();
    REQUIRE(testbench::is_value_equal(random_raw, 3789127393u), "Pseudo-random number 4 was %u, expected %u", random_raw, 3789127393u);
    random_raw = random_rlcg.get_random_raw_forward();
    REQUIRE(testbench::is_value_equal(random_raw, 236503499u), "Pseudo-random number 5 was %u, expected %u", random_raw, 236503499u);
}

TEST(random_rlcg, function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    double random_exclusive = round(random_rlcg.get_random_exclusive_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.221135), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.221135);
    random_exclusive = round(random_rlcg.get_random_exclusive_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.627183), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.627183);
    random_exclusive = round(random_rlcg.get_random_exclusive_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.039085);
    random_exclusive = round(random_rlcg.get_random_exclusive_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.882225);
    random_exclusive = round(random_rlcg.get_random_exclusive_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.055065);
}

TEST(random_rlcg, function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    double random_exclusive_top = round(random_rlcg.get_random_exclusive_top_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.221135), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.221135);
    random_exclusive_top = round(random_rlcg.get_random_exclusive_top_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.627183), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.627183);
    random_exclusive_top = round(random_rlcg.get_random_exclusive_top_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.039085);
    random_exclusive_top = round(random_rlcg.get_random_exclusive_top_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.882225);
    random_exclusive_top = round(random_rlcg.get_random_exclusive_top_forward());
    REQUIRE(testbench::is_value_equal(random_exclusive_top, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.055065);
}

TEST(random_rlcg, function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    double random_inclusive = round(random_rlcg.get_random_inclusive_forward());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.221135), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.221135);
    random_inclusive = round(random_rlcg.get_random_inclusive_forward());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.627183), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.627183);
    random_inclusive = round(random_rlcg.get_random_inclusive_forward());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.039085);
    random_inclusive = round(random_rlcg.get_random_inclusive_forward());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.882225);
    random_inclusive = round(random_rlcg.get_random_inclusive_forward());
    REQUIRE(testbench::is_value_equal(random_inclusive, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.055065);
}

TEST(random_rlcg, function, get_random_bounded_long) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    unsigned int random = random_rlcg.get_random_forward(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 0u), "Pseudo-random number 1 was %u, expected %u", random, 0u);
    random = random_rlcg.get_random_forward(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 2 was %u, expected %u", random, 1u);
    random = random_rlcg.get_random_forward(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 3 was %u, expected %u", random, 1u);
    random = random_rlcg.get_random_forward(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 4 was %u, expected %u", random, 1u);
    random = random_rlcg.get_random_forward(0u, 1u);
    REQUIRE(testbench::is_value_equal(random, 1u), "Pseudo-random number 5 was %u, expected %u", random, 1u);
}

TEST(random_rlcg, function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    double random = round(random_rlcg.get_random_forward(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.778865), "Pseudo-random number 1 was %lf, expected %lf", random, -0.778865);
    random = round(random_rlcg.get_random_forward(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.372817), "Pseudo-random number 2 was %lf, expected %lf", random, -0.372817);
    random = round(random_rlcg.get_random_forward(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.960915), "Pseudo-random number 3 was %lf, expected %lf", random, -0.960915);
    random = round(random_rlcg.get_random_forward(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.117775), "Pseudo-random number 4 was %lf, expected %lf", random, -0.117775);
    random = round(random_rlcg.get_random_forward(-1.0, 0.0));
    REQUIRE(testbench::is_value_equal(random, -0.944935), "Pseudo-random number 5 was %lf, expected %lf", random, -0.944935);
}

TEST(random_rlcg, evaluation, random_numbers_forward) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    int random_bias = 0;
    for (unsigned long long int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_rlcg.get_random_forward(0u, 1llu) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, -418), "Pseudo-random number generator bias was %d, expected %d", random_bias, -418);
}

TEST(random_rlcg, evaluation, random_numbers_inverse) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    int random_bias = 0;
    for (unsigned long long int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_rlcg.get_random_inverse(0u, 1llu) == 0) ? +1 : -1;
    }
    REQUIRE(testbench::is_value_equal(random_bias, 776), "Pseudo-random number generator bias was %d, expected %d", random_bias, 776);
}

TEST(random_rlcg, evaluation, random_numbers_forward_inverse) {
    gtl::random_rlcg random_rlcg;
    random_rlcg.seed(0x01234567);
    unsigned long long int random_sequence[1001];
    for (long long int iteration = 0; iteration < 1000; ++iteration) {
        random_sequence[iteration] = random_rlcg.get_random_raw_forward();
    }
    for (long long int iteration = 999; iteration >= 0; --iteration) {
        unsigned long long int random = random_rlcg.get_random_raw_inverse();
        REQUIRE(testbench::is_value_equal(random_sequence[iteration], random), "Pseudo-random number %lld was %llu, expected %llu", iteration, random, random_sequence[iteration]);
    }
}



