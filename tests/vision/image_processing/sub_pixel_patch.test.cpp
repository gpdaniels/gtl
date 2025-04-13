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
#include <testbench/comparison.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <vision/image_processing/sub_pixel_patch>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(sub_pixel_patch, function, square_00) {
    constexpr static const unsigned int data_width = 5;
    constexpr static const unsigned int data_height = 5;
    constexpr static const unsigned char data[data_height][data_width] = {
        {   0,   0,   0,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0 }
    };

    constexpr static const unsigned int start_x = 1;
    constexpr static const unsigned int start_y = 1;

    constexpr static const unsigned int patch_width = 3;
    constexpr static const unsigned int patch_height = 3;
    constexpr static const float patch_expected[patch_height][patch_width] = {
        { 254, 254,   0 },
        { 254, 254,   0 },
        {   0,   0,   0 }
    };

    float patch[patch_height][patch_width] = {};
    gtl::sub_pixel_patch<patch_width, patch_height>(&data[start_x][start_y], data_width, 0.0, 0.0, &patch[0][0]);

    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[0][0]), static_cast<double>(patch_expected[0][1]), static_cast<double>(patch_expected[0][2]), static_cast<double>(patch[0][0]), static_cast<double>(patch[0][1]), static_cast<double>(patch[0][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ] = [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[1][0]), static_cast<double>(patch_expected[1][1]), static_cast<double>(patch_expected[1][2]), static_cast<double>(patch[1][0]), static_cast<double>(patch[1][1]), static_cast<double>(patch[1][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[2][0]), static_cast<double>(patch_expected[2][1]), static_cast<double>(patch_expected[2][2]), static_cast<double>(patch[2][0]), static_cast<double>(patch[2][1]), static_cast<double>(patch[2][2]));

    for (unsigned int y = 0; y < patch_height; ++y) {
        for (unsigned int x = 0; x < patch_width; ++x) {
            REQUIRE(testbench::is_value_approx(patch[y][x], patch_expected[y][x], 1E-6f));
        }
    }
}


TEST(sub_pixel_patch, function, square_55) {
    constexpr static const unsigned int data_width = 5;
    constexpr static const unsigned int data_height = 5;
    constexpr static const unsigned char data[data_height][data_width] = {
        {   0,   0,   0,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0 }
    };

    constexpr static const unsigned int start_x = 1;
    constexpr static const unsigned int start_y = 1;

    constexpr static const unsigned int patch_width = 3;
    constexpr static const unsigned int patch_height = 3;
    constexpr static const float patch_expected[patch_height][patch_width] = {
        { 254.0, 127.0,   0.0 },
        { 127.0,  63.5,   0.0 },
        {   0.0,   0.0,   0.0 }
    };

    float patch[patch_height][patch_width] = {};
    gtl::sub_pixel_patch<patch_width, patch_height>(&data[start_x][start_y], data_width, 0.5, 0.5, &patch[0][0]);

    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[0][0]), static_cast<double>(patch_expected[0][1]), static_cast<double>(patch_expected[0][2]), static_cast<double>(patch[0][0]), static_cast<double>(patch[0][1]), static_cast<double>(patch[0][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ] = [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[1][0]), static_cast<double>(patch_expected[1][1]), static_cast<double>(patch_expected[1][2]), static_cast<double>(patch[1][0]), static_cast<double>(patch[1][1]), static_cast<double>(patch[1][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[2][0]), static_cast<double>(patch_expected[2][1]), static_cast<double>(patch_expected[2][2]), static_cast<double>(patch[2][0]), static_cast<double>(patch[2][1]), static_cast<double>(patch[2][2]));

    for (unsigned int y = 0; y < patch_height; ++y) {
        for (unsigned int x = 0; x < patch_width; ++x) {
            REQUIRE(testbench::is_value_approx(patch[y][x], patch_expected[y][x], 1E-6f));
        }
    }
}

TEST(sub_pixel_patch, function, square_11) {
    constexpr static const unsigned int data_width = 5;
    constexpr static const unsigned int data_height = 5;
    constexpr static const unsigned char data[data_height][data_width] = {
        {   0,   0,   0,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0, 254, 254,   0,   0 },
        {   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0 }
    };

    constexpr static const unsigned int start_x = 1;
    constexpr static const unsigned int start_y = 1;

    constexpr static const unsigned int patch_width = 3;
    constexpr static const unsigned int patch_height = 3;
    constexpr static const float patch_expected[patch_height][patch_width] = {
        {   0,   0,   0 },
        {   0, 254, 254 },
        {   0, 254, 254 }
    };

    float patch[patch_height][patch_width] = {};
    gtl::sub_pixel_patch<patch_width, patch_height>(&data[start_x][start_y], data_width, 1.0, 1.0, &patch[0][0]);

    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[0][0]), static_cast<double>(patch_expected[0][1]), static_cast<double>(patch_expected[0][2]), static_cast<double>(patch[0][0]), static_cast<double>(patch[0][1]), static_cast<double>(patch[0][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ] = [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[1][0]), static_cast<double>(patch_expected[1][1]), static_cast<double>(patch_expected[1][2]), static_cast<double>(patch[1][0]), static_cast<double>(patch[1][1]), static_cast<double>(patch[1][2]));
    PRINT("[ % 6.1f % 6.1f % 6.1f ]   [ % 6.1f % 6.1f % 6.1f ]\n", static_cast<double>(patch_expected[2][0]), static_cast<double>(patch_expected[2][1]), static_cast<double>(patch_expected[2][2]), static_cast<double>(patch[2][0]), static_cast<double>(patch[2][1]), static_cast<double>(patch[2][2]));

    for (unsigned int y = 0; y < patch_height; ++y) {
        for (unsigned int x = 0; x < patch_width; ++x) {
            REQUIRE(testbench::is_value_approx(patch[y][x], patch_expected[y][x], 1E-6f));
        }
    }
}
