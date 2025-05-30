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

#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <ai/hmm>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(hmm, traits, standard) {
    REQUIRE((std::is_pod<gtl::hmm<float>>::value == false));

    REQUIRE((std::is_trivial<gtl::hmm<float>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::hmm<float>>::value == false));

    REQUIRE((std::is_standard_layout<gtl::hmm<float>>::value == true));
}

TEST(hmm, constructor, empty) {
    gtl::hmm<float> hmm;
    testbench::do_not_optimise_away(hmm);
}

TEST(hmm, constructor, arguments) {
    gtl::hmm<float> hmm = gtl::hmm<float>(10, 100);
    testbench::do_not_optimise_away(hmm);
}

TEST(hmm, function, train) {
    gtl::hmm<float> hmm_1 = gtl::hmm<float>(2, 2);
    unsigned long long int iterations_1 = hmm_1.train({ { 0, 1, 0 }, { 0, 1 }, { 0, 1, 0, 1, 0 }, { 1, 0, 1, 0 } }, 1000, 1);
    REQUIRE(iterations_1 == 1);
    gtl::hmm<float> hmm_11 = gtl::hmm<float>(2, 2);
    unsigned long long int iterations_11 = hmm_11.train({ { 0, 1, 0 }, { 0, 1 }, { 0, 1, 0, 1, 0 }, { 1, 0, 1, 0 } }, 1000);
    REQUIRE(iterations_11 > 10);
    gtl::hmm<float> hmm_2 = gtl::hmm<float>(2, 2);
    unsigned long long int iterations_2 = hmm_2.train({ { 0, 1, 0 }, { 0, 1 }, { 0, 1, 0, 1, 0 }, { 1, 0, 1, 0 } }, 2);
    REQUIRE(iterations_2 == 2);
}

TEST(hmm, function, process) {
    gtl::hmm<float> hmm = gtl::hmm<float>(2, 2);
    hmm.train({ { 0, 1, 0 }, { 0, 1 }, { 0, 1, 0, 1, 0 }, { 1, 0, 1, 0 } }, 1000);
    float log_probability = hmm.process({ 1, 0, 1 });
    REQUIRE(log_probability > -2);
}

TEST(hmm, function, predict) {
    gtl::hmm<float> hmm = gtl::hmm<float>(2, 2);
    hmm.train({ { 0, 1, 0 }, { 0, 1 }, { 0, 1, 0, 1, 0 }, { 1, 0, 1, 0 } }, 1000);
    unsigned long long int next_zero = hmm.predict({ 0, 1, 0, 1 });
    REQUIRE(next_zero == 0);
    unsigned long long int next_one = hmm.predict({ 1, 0 });
    REQUIRE(next_one == 1);
}

TEST(hmm, evaluate, ascending) {
    gtl::hmm<float> hmm = gtl::hmm<float>(10, 10);

    // A number of valid sequences, (notice all are ascending).
    // These are designed so every ascending transition is covered.
    std::vector<std::vector<unsigned long long int>> observations_training = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        { 0, 2, 4, 6, 8 },
        { 1, 3, 5, 7, 9 },
        { 0, 3, 6, 9 },
        { 1, 4, 7 },
        { 2, 5, 8 },
        { 0, 4, 8 },
        { 1, 5, 9 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
        { 0, 5 },
        { 1, 6 },
        { 2, 7 },
        { 3, 8 },
        { 4, 9 },
        { 0, 6 },
        { 1, 7 },
        { 2, 8 },
        { 3, 9 },
        { 0, 7 },
        { 1, 8 },
        { 2, 9 },
        { 0, 8 },
        { 1, 9 },
        { 0, 9 },
        { 0 },
        { 1 },
        { 2 },
        { 3 },
        { 4 },
        { 5 },
        { 6 },
        { 7 },
        { 8 },
        { 9 }
    };

    // A number of valid sequences, (notice all are ascending).
    std::vector<std::vector<unsigned long long int>> observations_valid = {
        { 0, 1, 2, 8 },
        { 0, 5, 6, 7 },
        { 5 }
    };

    // A number of invalid sequences, (notice none are perfectly ascending).
    std::vector<std::vector<unsigned long long int>> observations_invalid = {
        { 0, 1, 2, 3, 4, 0, 6, 7, 8, 9 },
        { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
        { 8, 6, 4, 2, 0 },
        { 0, 2, 1 },
        { 0, 2, 4, 3, 2, 1 },
        { 0, 4, 2, 1 },
        { 1, 2, 5, 7, 3 },
        { 0, 0, 0, 0 }
    };

    unsigned long long int iterations = hmm.train(observations_training);
    PRINT("Iterations: %llu\n", iterations);

    for (auto training : observations_training) {
        PRINT("Training sequence: log(%f)\n", static_cast<double>(hmm.process(training)));
        REQUIRE(hmm.process(training) > -15, "Probablity of training sequence too low: log(%f)\n", static_cast<double>(hmm.process(training)));
    }
    for (auto valid : observations_valid) {
        PRINT("Valid sequence: log(%f)\n", static_cast<double>(hmm.process(valid)));
        REQUIRE(hmm.process(valid) > -35, "Probablity of valid sequence too low: log(%f)\n", static_cast<double>(hmm.process(valid)));
    }
    for (auto invalid : observations_invalid) {
        PRINT("Invalid sequence: log(%f)\n", static_cast<double>(hmm.process(invalid)));
        REQUIRE(hmm.process(invalid) < -70, "Probablity of invalid sequence too high: log(%f)\n", static_cast<double>(hmm.process(invalid)));
    }
}

TEST(hmm, evaluate, multiple_models) {
    gtl::hmm<float> hmm_zeros = gtl::hmm<float>(2, 1);
    gtl::hmm<float> hmm_ones = gtl::hmm<float>(2, 1);

    hmm_zeros.train({ { 0 },
                      { 0, 0 },
                      { 0, 0, 0 } });

    hmm_ones.train({ { 1 },
                     { 1, 1 },
                     { 1, 1, 1 } });

    std::vector<unsigned long long int> test_zeros = { 0, 0, 0, 0 };
    REQUIRE(hmm_zeros.process(test_zeros) > hmm_ones.process(test_zeros));

    std::vector<unsigned long long int> test_ones = { 1, 1, 1, 1 };
    REQUIRE(hmm_zeros.process(test_ones) < hmm_ones.process(test_ones));
}
