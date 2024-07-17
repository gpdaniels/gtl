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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <vision/consensus/consensus>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#include <vision/consensus/evaluate/inlier_support>
#include <vision/consensus/sample/random>

class xy final {
public:
    float x, y;
};

class line final {
public:
    float gradient;
    float intercept;
};

class line_estimator final
    : public gtl::estimator<xy, 2, line, 1> {
public:
    virtual std::size_t generate_models(const data_type* const __restrict data, std::size_t data_size, model_type* const __restrict models) override final {
        REQUIRE(data_size == 2);
        models[0].gradient = (data[1].y - data[0].y) / (data[1].x - data[0].x);
        models[0].intercept = data[1].y - models[0].gradient * data[1].x;
        return 1;
    }
    virtual void compute_residuals(const data_type* const __restrict data, std::size_t data_size, const model_type& model, float* const __restrict residuals) override final {
        const float denominator = std::sqrt(model.gradient * model.gradient + 1.0f);
        for (std::size_t i = 0; i < data_size; ++i) {
            residuals[i] = std::abs(-model.gradient * data[i].x + data[i].y - model.intercept) / denominator;
        }
    }
};

TEST(consensus, evaluate, line) {

    class random_pcg final {
    private:
        unsigned long long int state = 0x853C49E6748FEA9Bull;
        unsigned long long int increment = 0xDA3E39CB94B95BDBull;
    private:
        unsigned int get_random_raw() {
            unsigned long long int state_previous = this->state;
            this->state = state_previous * 0x5851F42D4C957F2Dull + this->increment;
            unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            int rotation = state_previous >> 59u;
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((-rotation) & 31));
        }
    public:
        float get_random_exclusive_top() {
            return static_cast<float>(this->get_random_raw()) * (1.0f / static_cast<float>(1ull << 32));
        }
    };

    constexpr static const std::size_t data_size = 2000;
    random_pcg rng;

    const float gradient = 1.234f;
    const float intercept = 5.6789f;

    // Create a dataset with half the points fitting the line (y = 1.234x + 5.6789) and half noise.
    xy data[data_size];
    for (std::size_t i = 0; i < data_size; ++i) {
        if (i % 2 == 0) {
            const float line_noise_x = rng.get_random_exclusive_top() * 0.1f;
            const float line_noise_y = rng.get_random_exclusive_top() * 0.1f;
            data[i] = xy{ static_cast<float>(i) + line_noise_x, static_cast<float>(i) * gradient + intercept + line_noise_y };
        }
        else {
            const float random_noise_x = rng.get_random_exclusive_top() * data_size;
            const float random_noise_y = rng.get_random_exclusive_top() * data_size;
            data[i] = xy{ random_noise_x, random_noise_y };
        }
    }

    const float probability_failure = 0.01f;
    const float inlier_ratio = 0.4f;
    const std::size_t iterations_minimum = 0;
    const std::size_t iterations_maximum = 100;
    const float residual_thresdhold = 0.1f;

    gtl::random<2> random;
    line_estimator estimator;
    gtl::inlier_support inlier_support(residual_thresdhold);

    gtl::consensus<gtl::random<2>, line_estimator, gtl::inlier_support> consensus(
        random,
        estimator,
        inlier_support,
        inlier_ratio,
        probability_failure,
        iterations_minimum,
        iterations_maximum
    );

    float residuals[data_size];
    std::size_t inliers[data_size];
    std::size_t inliers_size = 0;
    line best_model;
    REQUIRE(consensus.estimate(data, data_size, residuals, inliers, inliers_size, best_model));

    REQUIRE(testbench::is_value_approx(best_model.gradient, gradient, 0.05f));
    REQUIRE(testbench::is_value_approx(best_model.intercept, intercept, 0.05f));
    REQUIRE(testbench::is_value_approx(inliers_size, data_size / 2, static_cast<std::size_t>(data_size * 0.01f)));
}
