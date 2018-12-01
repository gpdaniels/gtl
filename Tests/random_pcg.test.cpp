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
#include <comparison.tests.hpp>
#include <macro.tests.hpp>

#include <random_pcg>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::random_pcg) >= 16ull, "sizeof(gtl::random_pcg) = %ld, expected >= %lld", sizeof(gtl::random_pcg), 16ull);

    REQUIRE(std::is_pod<gtl::random_pcg>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::random_pcg>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::random_pcg>::value == true, "Expected std::is_trivially_copyable to be true.");

    REQUIRE(std::is_standard_layout<gtl::random_pcg>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::random_pcg random_pcg;
    do_not_optimise_away(random_pcg);
}

TEST(constructor, seed) {
    gtl::random_pcg random_pcg(0x01234567);
    do_not_optimise_away(random_pcg);
}

TEST(function, seed) {
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
}

TEST(function, get_random_raw) {
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    unsigned int random_raw = random_pcg.get_random_raw();
    REQUIRE(comparison::is_equal(random_raw, 1350580380u), "Pseudo-random number 1 was %u, expected %u", random_raw, 1350580380u);
    random_raw = random_pcg.get_random_raw();
    REQUIRE(comparison::is_equal(random_raw, 206545039u), "Pseudo-random number 2 was %u, expected %u", random_raw, 206545039u);
    random_raw = random_pcg.get_random_raw();
    REQUIRE(comparison::is_equal(random_raw, 167867113u), "Pseudo-random number 3 was %u, expected %u", random_raw, 167867113u);
    random_raw = random_pcg.get_random_raw();
    REQUIRE(comparison::is_equal(random_raw, 3789127393u), "Pseudo-random number 4 was %u, expected %u", random_raw, 3789127393u);
    random_raw = random_pcg.get_random_raw();
    REQUIRE(comparison::is_equal(random_raw, 236503499u), "Pseudo-random number 5 was %u, expected %u", random_raw, 236503499u);
}

TEST(function, get_random_exclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    double random_exclusive = round(random_pcg.get_random_exclusive());
    REQUIRE(comparison::is_equal(random_exclusive, 0.314456), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive, 0.314456);
    random_exclusive = round(random_pcg.get_random_exclusive());
    REQUIRE(comparison::is_equal(random_exclusive, 0.048090), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive, 0.048090);
    random_exclusive = round(random_pcg.get_random_exclusive());
    REQUIRE(comparison::is_equal(random_exclusive, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive, 0.039085);
    random_exclusive = round(random_pcg.get_random_exclusive());
    REQUIRE(comparison::is_equal(random_exclusive, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive, 0.882225);
    random_exclusive = round(random_pcg.get_random_exclusive());
    REQUIRE(comparison::is_equal(random_exclusive, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive, 0.055065);
}

TEST(function, get_random_exclusive_top) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    double random_exclusive_top = round(random_pcg.get_random_exclusive_top());
    REQUIRE(comparison::is_equal(random_exclusive_top, 0.314456), "Pseudo-random number 1 was %lf, expected %lf", random_exclusive_top, 0.314456);
    random_exclusive_top = round(random_pcg.get_random_exclusive_top());
    REQUIRE(comparison::is_equal(random_exclusive_top, 0.048090), "Pseudo-random number 2 was %lf, expected %lf", random_exclusive_top, 0.048090);
    random_exclusive_top = round(random_pcg.get_random_exclusive_top());
    REQUIRE(comparison::is_equal(random_exclusive_top, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_exclusive_top, 0.039085);
    random_exclusive_top = round(random_pcg.get_random_exclusive_top());
    REQUIRE(comparison::is_equal(random_exclusive_top, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_exclusive_top, 0.882225);
    random_exclusive_top = round(random_pcg.get_random_exclusive_top());
    REQUIRE(comparison::is_equal(random_exclusive_top, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_exclusive_top, 0.055065);
}

TEST(function, get_random_inclusive) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    double random_inclusive = round(random_pcg.get_random_inclusive());
    REQUIRE(comparison::is_equal(random_inclusive, 0.314456), "Pseudo-random number 1 was %lf, expected %lf", random_inclusive, 0.314456);
    random_inclusive = round(random_pcg.get_random_inclusive());
    REQUIRE(comparison::is_equal(random_inclusive, 0.048090), "Pseudo-random number 2 was %lf, expected %lf", random_inclusive, 0.048090);
    random_inclusive = round(random_pcg.get_random_inclusive());
    REQUIRE(comparison::is_equal(random_inclusive, 0.039085), "Pseudo-random number 3 was %lf, expected %lf", random_inclusive, 0.039085);
    random_inclusive = round(random_pcg.get_random_inclusive());
    REQUIRE(comparison::is_equal(random_inclusive, 0.882225), "Pseudo-random number 4 was %lf, expected %lf", random_inclusive, 0.882225);
    random_inclusive = round(random_pcg.get_random_inclusive());
    REQUIRE(comparison::is_equal(random_inclusive, 0.055065), "Pseudo-random number 5 was %lf, expected %lf", random_inclusive, 0.055065);
}

TEST(function, get_random_bounded_long) {
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    unsigned int random = random_pcg.get_random(0u, 1u);
    REQUIRE(comparison::is_equal(random, 0u), "Pseudo-random number 1 was %u, expected %u", random, 0u);
    random = random_pcg.get_random(0u, 1u);
    REQUIRE(comparison::is_equal(random, 1u), "Pseudo-random number 2 was %u, expected %u", random, 1u);
    random = random_pcg.get_random(0u, 1u);
    REQUIRE(comparison::is_equal(random, 1u), "Pseudo-random number 3 was %u, expected %u", random, 1u);
    random = random_pcg.get_random(0u, 1u);
    REQUIRE(comparison::is_equal(random, 1u), "Pseudo-random number 4 was %u, expected %u", random, 1u);
    random = random_pcg.get_random(0u, 1u);
    REQUIRE(comparison::is_equal(random, 1u), "Pseudo-random number 5 was %u, expected %u", random, 1u);
}

TEST(function, get_random_bounded_double) {
    auto round = [](double value) -> double { return std::round(value * 1000000.0) / 1000000.0; };
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    double random = round(random_pcg.get_random(-1.0, 0.0));
    REQUIRE(comparison::is_equal(random, -0.685544), "Pseudo-random number 1 was %lf, expected %lf", random, -0.685544);
    random = round(random_pcg.get_random(-1.0, 0.0));
    REQUIRE(comparison::is_equal(random, -0.951910), "Pseudo-random number 2 was %lf, expected %lf", random, -0.951910);
    random = round(random_pcg.get_random(-1.0, 0.0));
    REQUIRE(comparison::is_equal(random, -0.960915), "Pseudo-random number 3 was %lf, expected %lf", random, -0.960915);
    random = round(random_pcg.get_random(-1.0, 0.0));
    REQUIRE(comparison::is_equal(random, -0.117775), "Pseudo-random number 4 was %lf, expected %lf", random, -0.117775);
    random = round(random_pcg.get_random(-1.0, 0.0));
    REQUIRE(comparison::is_equal(random, -0.944935), "Pseudo-random number 5 was %lf, expected %lf", random, -0.944935);
}

TEST(evaluation, random_numbers) {
    gtl::random_pcg random_pcg;
    random_pcg.seed(0x01234567);
    int random_bias = 0;
    for (unsigned int iteration = 0; iteration < 1000000; ++iteration) {
        random_bias += (random_pcg.get_random(0u, 1u) == 0) ? +1 : -1;
    }
    REQUIRE(comparison::is_equal(random_bias, -98), "Pseudo-random number generator bias was %d, expected %d", random_bias, -98);
}
