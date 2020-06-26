/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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

TEST(random_romu_32, function, seed) {
    gtl::random_romu_32 random_romu_32;
    random_romu_32.seed(0x01234567);
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
