/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <benchmark.tests.hpp>
#include <macro.tests.hpp>

#include <random_mt>

#include <cmath>
#include <type_traits>

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::random_mt) >= 2500ull, "sizeof(gtl::random_mt) = %ld, expected >= %lld", sizeof(gtl::random_mt), 2500ull);

    REQUIRE(std::is_pod<gtl::random_mt>::value == true, "Expected std::is_pod to be true.");

    REQUIRE(std::is_trivial<gtl::random_mt>::value == true, "Expected std::is_trivial to be true.");

    REQUIRE(std::is_trivially_copyable<gtl::random_mt>::value == true, "Expected std::is_trivially_copyable to be true.");

    REQUIRE(std::is_standard_layout<gtl::random_mt>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::random_mt random_mt;
    DoNotOptimiseAway(random_mt);
}

TEST(constructor, seed_value) {
    gtl::random_mt random_mt(0x01234567);
    DoNotOptimiseAway(random_mt);
}

TEST(constructor, seed_array) {
    unsigned int seed_values[5] = { 0x01234567, 0x12345678, 0x23456789, 0x34567890, 0x45678901 };
    gtl::random_mt random_mt(seed_values, 5);
    DoNotOptimiseAway(random_mt);
}

TEST(function, seed_value) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
}

TEST(function, seed_array) {
    unsigned int seed_values[5] = { 0x01234567, 0x12345678, 0x23456789, 0x34567890, 0x45678901 };
    gtl::random_mt random_mt;
    random_mt.seed(seed_values, 5);
}

TEST(function, get_random_raw) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    unsigned int random_raw = random_mt.get_random_raw();
    REQUIRE(random_raw == 1802874368, "Pseudo-random number 1 was %u, expected %u", random_raw, 1802874368u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(random_raw == 453914126, "Pseudo-random number 2 was %u, expected %u", random_raw, 453914126u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(random_raw == 2896676756, "Pseudo-random number 3 was %u, expected %u", random_raw, 2896676756u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(random_raw == 1924626451, "Pseudo-random number 4 was %u, expected %u", random_raw, 1924626451u);
    random_raw = random_mt.get_random_raw();
    REQUIRE(random_raw == 1073351711, "Pseudo-random number 5 was %u, expected %u", random_raw, 1073351711u);
}

TEST(function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(random_exclusive == 0.419764, "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.419764);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(random_exclusive == 0.105685, "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.105685);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(random_exclusive == 0.674435, "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.674435);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(random_exclusive == 0.448112, "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.448112);
    random_exclusive = round(random_mt.get_random_exclusive());
    REQUIRE(random_exclusive == 0.249909, "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.249909);
}

TEST(function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(random_exclusive_top == 0.419764, "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.419764);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(random_exclusive_top == 0.105685, "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.105685);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(random_exclusive_top == 0.674435, "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.674435);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(random_exclusive_top == 0.448112, "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.448112);
    random_exclusive_top = round(random_mt.get_random_exclusive_top());
    REQUIRE(random_exclusive_top == 0.249909, "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.249909);
}

TEST(function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(random_inclusive == 0.419764, "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.419764);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(random_inclusive == 0.105685, "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.105685);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(random_inclusive == 0.674435, "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.674435);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(random_inclusive == 0.448112, "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.448112);
    random_inclusive = round(random_mt.get_random_inclusive());
    REQUIRE(random_inclusive == 0.249909, "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.249909);
}

TEST(function, get_random_bounded_long) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    unsigned int random = random_mt.get_random(0u, 1u);
    REQUIRE(random == 0, "Pseudo-random number 1 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(random == 0, "Pseudo-random number 2 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(random == 0, "Pseudo-random number 3 was %u, expected %u", random, 0u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(random == 1, "Pseudo-random number 4 was %u, expected %u", random, 1u);
    random = random_mt.get_random(0u, 1u);
    REQUIRE(random == 1, "Pseudo-random number 5 was %u, expected %u", random, 1u);
}

TEST(function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    double random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(random == -0.580236, "Pseudo-random number 1 was %lf, expected %lf", random, -0.580236);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(random == -0.894315, "Pseudo-random number 2 was %lf, expected %lf", random, -0.894315);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(random == -0.325565, "Pseudo-random number 3 was %lf, expected %lf", random, -0.325565);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(random == -0.551888, "Pseudo-random number 4 was %lf, expected %lf", random, -0.551888);
    random = round(random_mt.get_random(-1.0, 0.0));
    REQUIRE(random == -0.750091, "Pseudo-random number 5 was %lf, expected %lf", random, -0.750091);
}

TEST(evaluation, random_numbers) {
    gtl::random_mt random_mt;
    random_mt.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_mt.get_random(0u, 1u) == 0) ? +1 : -1;
    }
    REQUIRE(random_bias == 74, "Pseudo-random number generator bias was %d, expected %d", random_bias, 74);
}
