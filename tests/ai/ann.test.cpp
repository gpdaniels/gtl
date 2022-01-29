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

#include <ai/ann>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(ann, traits, standard) {
    REQUIRE((std::is_pod<gtl::ann<float>>::value == false));

    REQUIRE((std::is_trivial<gtl::ann<float>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::ann<float>>::value == false));

    REQUIRE((std::is_standard_layout<gtl::ann<float>>::value == true));
}

TEST(ann, constructor, empty) {
    gtl::ann<float> ann;
    testbench::do_not_optimise_away(ann);
}

TEST(ann, constructor, minimal_arguments) {
    gtl::ann<float> ann({0, 0});
    testbench::do_not_optimise_away(ann);
}

TEST(ann, constructor, maximal_arguments) {
    gtl::ann<float> ann({0, 0}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::sigmoid);
    testbench::do_not_optimise_away(ann);
}

TEST(ann, function, train) {
    gtl::ann<float> ann({0, 0});
    ann.train({}, {});
}

TEST(ann, function, process) {
    gtl::ann<float> ann({0, 0});
    std::vector<float> output = ann.process({});
}

TEST(ann, evaluate, or_gate) {
    gtl::ann<float> ann = gtl::ann<float>({2, 5, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {0.0}});
    data.push_back({{0.0, 1.0}, {1.0}});
    data.push_back({{1.0, 0.0}, {1.0}});
    data.push_back({{1.0, 1.0}, {1.0}});

    for (unsigned int iterations = 0; iterations < 4 * 10000; ++iterations) {
        ann.train(data[iterations % 4].first, data[iterations % 4].second);
    }

    std::vector<float> output;
    output = ann.process(data[0].first);
    REQUIRE(testbench::is_value_approx(output[0], data[0].second[0], 1e-6f), "0 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[0].second[0]));

    output = ann.process(data[1].first);
    REQUIRE(testbench::is_value_approx(output[0], data[1].second[0], 1e-6f), "0 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[1].second[0]));

    output = ann.process(data[2].first);
    REQUIRE(testbench::is_value_approx(output[0], data[2].second[0], 1e-6f), "1 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[2].second[0]));

    output = ann.process(data[3].first);
    REQUIRE(testbench::is_value_approx(output[0], data[3].second[0], 1e-6f), "1 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[3].second[0]));
}

TEST(ann, evaluate, and_gate) {
    gtl::ann<float> ann = gtl::ann<float>({2, 5, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {0.0}});
    data.push_back({{0.0, 1.0}, {0.0}});
    data.push_back({{1.0, 0.0}, {0.0}});
    data.push_back({{1.0, 1.0}, {1.0}});

    for (unsigned int iterations = 0; iterations < 4 * 10000; ++iterations) {
        ann.train(data[iterations % 4].first, data[iterations % 4].second);
    }

    std::vector<float> output;
    output = ann.process(data[0].first);
    REQUIRE(testbench::is_value_approx(output[0], data[0].second[0], 1e-6f), "0 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[0].second[0]));

    output = ann.process(data[1].first);
    REQUIRE(testbench::is_value_approx(output[0], data[1].second[0], 1e-6f), "0 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[1].second[0]));

    output = ann.process(data[2].first);
    REQUIRE(testbench::is_value_approx(output[0], data[2].second[0], 1e-6f), "1 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[2].second[0]));

    output = ann.process(data[3].first);
    REQUIRE(testbench::is_value_approx(output[0], data[3].second[0], 1e-6f), "1 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[3].second[0]));
}

TEST(ann, evaluate, nor_gate) {
    gtl::ann<float> ann = gtl::ann<float>({2, 5, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {1.0}});
    data.push_back({{0.0, 1.0}, {0.0}});
    data.push_back({{1.0, 0.0}, {0.0}});
    data.push_back({{1.0, 1.0}, {0.0}});

    for (unsigned int iterations = 0; iterations < 4 * 10000; ++iterations) {
        ann.train(data[iterations % 4].first, data[iterations % 4].second);
    }

    std::vector<float> output;
    output = ann.process(data[0].first);
    REQUIRE(testbench::is_value_approx(output[0], data[0].second[0], 1e-6f), "0 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[0].second[0]));

    output = ann.process(data[1].first);
    REQUIRE(testbench::is_value_approx(output[0], data[1].second[0], 1e-6f), "0 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[1].second[0]));

    output = ann.process(data[2].first);
    REQUIRE(testbench::is_value_approx(output[0], data[2].second[0], 1e-6f), "1 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[2].second[0]));

    output = ann.process(data[3].first);
    REQUIRE(testbench::is_value_approx(output[0], data[3].second[0], 1e-6f), "1 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[3].second[0]));
}

TEST(ann, evaluate, nand_gate) {
    gtl::ann<float> ann = gtl::ann<float>({2, 5, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {1.0}});
    data.push_back({{0.0, 1.0}, {1.0}});
    data.push_back({{1.0, 0.0}, {1.0}});
    data.push_back({{1.0, 1.0}, {0.0}});

    for (unsigned int iterations = 0; iterations < 4 * 10000; ++iterations) {
        ann.train(data[iterations % 4].first, data[iterations % 4].second);
    }

    std::vector<float> output;
    output = ann.process(data[0].first);
    REQUIRE(testbench::is_value_approx(output[0], data[0].second[0], 1e-6f), "0 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[0].second[0]));

    output = ann.process(data[1].first);
    REQUIRE(testbench::is_value_approx(output[0], data[1].second[0], 1e-6f), "0 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[1].second[0]));

    output = ann.process(data[2].first);
    REQUIRE(testbench::is_value_approx(output[0], data[2].second[0], 1e-6f), "1 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[2].second[0]));

    output = ann.process(data[3].first);
    REQUIRE(testbench::is_value_approx(output[0], data[3].second[0], 1e-6f), "1 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[3].second[0]));
}

TEST(ann, evaluate, xor_gate) {
    gtl::ann<float> ann = gtl::ann<float>({2, 5, 1});

    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {0.0}});
    data.push_back({{0.0, 1.0}, {1.0}});
    data.push_back({{1.0, 0.0}, {1.0}});
    data.push_back({{1.0, 1.0}, {0.0}});

    for (unsigned int iterations = 0; iterations < 4 * 10000; ++iterations) {
        ann.train(data[iterations % 4].first, data[iterations % 4].second);
    }

    std::vector<float> output;
    output = ann.process(data[0].first);
    REQUIRE(testbench::is_value_approx(output[0], data[0].second[0], 1e-6f), "0 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[0].second[0]));

    output = ann.process(data[1].first);
    REQUIRE(testbench::is_value_approx(output[0], data[1].second[0], 1e-6f), "0 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[1].second[0]));

    output = ann.process(data[2].first);
    REQUIRE(testbench::is_value_approx(output[0], data[2].second[0], 1e-6f), "1 0 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[2].second[0]));

    output = ann.process(data[3].first);
    REQUIRE(testbench::is_value_approx(output[0], data[3].second[0], 1e-6f), "1 1 => %f != %f\n", static_cast<double>(output[0]), static_cast<double>(data[3].second[0]));
}

TEST(ann, evaluate, learning_rate) {
    std::vector<std::pair<std::vector<float>, std::vector<float>>> data;
    data.push_back({{0.0, 0.0}, {0.0}});
    data.push_back({{0.0, 1.0}, {1.0}});
    data.push_back({{1.0, 0.0}, {1.0}});
    data.push_back({{1.0, 1.0}, {0.0}});

    auto train_network = [&data](gtl::ann<float>& ann, unsigned int& iterations) -> bool {
        bool learnt = true;
        for (iterations = 0; iterations < 4 * 10000; ++iterations) {
            ann.train(data[iterations % 4].first, data[iterations % 4].second);
            learnt = true;
            std::vector<float> output;
            output = ann.process(data[0].first);
            learnt &= testbench::is_value_approx(output[0], data[0].second[0], 1e-6f);
            output = ann.process(data[1].first);
            learnt &= testbench::is_value_approx(output[0], data[1].second[0], 1e-6f);
            output = ann.process(data[2].first);
            learnt &= testbench::is_value_approx(output[0], data[2].second[0], 1e-6f);
            output = ann.process(data[3].first);
            learnt &= testbench::is_value_approx(output[0], data[3].second[0], 1e-6f);
            if (learnt) {
                break;
            }
        }
        return learnt;
    };

    unsigned int iterations_none     = 0;
    unsigned int iterations_identity = 0;
    unsigned int iterations_step     = 0;
    unsigned int iterations_sigmoid  = 0;
    unsigned int iterations_softplus = 0;
    unsigned int iterations_tanh     = 0;
    unsigned int iterations_erf      = 0;

    gtl::ann<float> ann_none     = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::none);
    gtl::ann<float> ann_identity = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::identity);
    gtl::ann<float> ann_step     = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::step);
    gtl::ann<float> ann_sigmoid  = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::sigmoid);
    gtl::ann<float> ann_softplus = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::softplus);
    gtl::ann<float> ann_tanh     = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::tanh);
    gtl::ann<float> ann_erf      = gtl::ann<float>({2, 5, 1}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::erf);

    bool learnt_none     = train_network(ann_none, iterations_none);
    bool learnt_identity = train_network(ann_identity, iterations_identity);
    bool learnt_step     = train_network(ann_identity, iterations_step);
    bool learnt_sigmoid  = train_network(ann_sigmoid, iterations_sigmoid);
    bool learnt_softplus = train_network(ann_softplus, iterations_softplus);
    bool learnt_tanh     = train_network(ann_tanh, iterations_tanh);
    bool learnt_erf      = train_network(ann_erf, iterations_erf);

    PRINT("Evaluate learning speed: \n");
    PRINT(" none     -> %d in %d\n", learnt_none, iterations_none);
    PRINT(" identity -> %d in %d\n", learnt_identity, iterations_identity);
    PRINT(" step     -> %d in %d\n", learnt_step, iterations_step);
    PRINT(" sigmoid  -> %d in %d\n", learnt_sigmoid, iterations_sigmoid);
    PRINT(" softplus -> %d in %d\n", learnt_softplus, iterations_softplus);
    PRINT(" tanh     -> %d in %d\n", learnt_tanh, iterations_tanh);
    PRINT(" erf      -> %d in %d\n", learnt_erf, iterations_erf);

    // Require that the activation functions with sensible derivatives all succeed.
    REQUIRE(learnt_sigmoid);
    REQUIRE(learnt_softplus);
    REQUIRE(learnt_tanh);
    REQUIRE(learnt_erf);
}

TEST(ann, evaluate, adder) {
    #if !defined(NDEBUG)
        // This test is too slow in debug mode to do a decent amount of training.
        constexpr static const unsigned int max_iterations = 256 * 2;
    #else
        constexpr static const unsigned int max_iterations = 256 * 200;
    #endif
    
    gtl::ann<float> ann = gtl::ann<float>({8, 32, 32, 4}, gtl::ann<float>::initialisation_type::random, gtl::ann<float>::activation_type::sigmoid);

    for (unsigned int iterations = 0; iterations < max_iterations; ++iterations) {
        char input_lhs = (iterations>>4)&0b00001111;
        char input_rhs = (iterations>>0)&0b00001111;
        char target_bits = input_lhs + input_rhs;

        std::vector<float> input = {
            static_cast<float>(input_lhs >> 3 & 1), static_cast<float>(input_lhs >> 2 & 1), static_cast<float>(input_lhs >> 1 & 1), static_cast<float>(input_lhs >> 0 & 1),
            static_cast<float>(input_rhs >> 3 & 1), static_cast<float>(input_rhs >> 2 & 1), static_cast<float>(input_rhs >> 1 & 1), static_cast<float>(input_rhs >> 0 & 1)
        };
        std::vector<float> target = {
            static_cast<float>(target_bits >> 3 & 1), static_cast<float>(target_bits >> 2 & 1), static_cast<float>(target_bits >> 1 & 1), static_cast<float>(target_bits >> 0 & 1)
        };

        ann.train(input, target);
    }

    int incorrect = 0;

    for (unsigned int iterations = 0; iterations < 256; ++iterations) {

        char input_lhs = (iterations>>4)&0b00001111;
        char input_rhs = (iterations>>0)&0b00001111;
        char target_bits = input_lhs + input_rhs;

        std::vector<float> input = {
            static_cast<float>(input_lhs >> 3 & 1), static_cast<float>(input_lhs >> 2 & 1), static_cast<float>(input_lhs >> 1 & 1), static_cast<float>(input_lhs >> 0 & 1),
            static_cast<float>(input_rhs >> 3 & 1), static_cast<float>(input_rhs >> 2 & 1), static_cast<float>(input_rhs >> 1 & 1), static_cast<float>(input_rhs >> 0 & 1)
        };
        std::vector<float> target = {
            static_cast<float>(target_bits >> 3 & 1), static_cast<float>(target_bits >> 2 & 1), static_cast<float>(target_bits >> 1 & 1), static_cast<float>(target_bits >> 0 & 1)
        };

        std::vector<float> output;
        output = ann.process(input);

        if (((target[0] > 0.5f) != (output[0] > 0.5f)) || ((target[1] > 0.5f) != (output[1] > 0.5f)) || ((target[2] > 0.5f) != (output[2] > 0.5f)) || ((target[3] > 0.5f) != (output[3] > 0.5f))) {
            ++incorrect;
        }
    }

    PRINT("Incorrect: %d\n", incorrect);
}
