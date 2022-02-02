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
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <vision/feature/score/shi_tomasi_score>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#include <vision/feature/feature>

TEST(shi_tomasi_score, function, gradient_with_inverted_square) {
    // Create some data.
    constexpr static const unsigned int data_width = 128;
    constexpr static const unsigned int data_height = 64;
    unsigned char data[data_height][data_width] = {};
    for (unsigned int y = 0; y < data_height; ++y) {
        for (unsigned int x = 0; x < data_width; ++x) {
            data[y][x] = static_cast<unsigned char>(x + y);
            if ((x > data_width / 4) && (x < (data_width * 3) / 4) && (y > data_height / 4) && (y < (data_height * 3) / 4)) {
                data[y][x] = 0;
            }
        }
    }

    constexpr static const unsigned int features_count = 4;
    gtl::feature features[features_count] = {
        { 95, 19, 0.93951416f, 0.0f }, { 35, 47, 0.932922363f, 0.0f }, { 95, 47, 33.5926666f, 0.0f }, { 35, 19, 0.0f, 0.0f }
    };

    for (unsigned int i = 0; i < features_count; ++i) {
        const float response = gtl::shi_tomasi_score(&data[features[i].y][features[i].x], data_width);
        REQUIRE(testbench::is_value_equal(features[i].response, response));
    }
}
